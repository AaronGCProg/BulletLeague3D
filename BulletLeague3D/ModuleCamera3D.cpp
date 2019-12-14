#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "ModulePlayer.h"
#include "PhysVehicle3D.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		cameraDebug = !cameraDebug;

	if (App->input->GetMouseButton(3) == KEY_DOWN)
		lookAtBall = !lookAtBall;

	if (cameraDebug)
	{
		//Debug Cam
		
		vec3 newPos(0,0,0);
		float speed = 3.0f * dt;
		if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 8.0f * dt;

		if(App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
		if(App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

		if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
		if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


		if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
		if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

		Position += newPos;
		Reference += newPos;

		// Mouse motion ----------------

		if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			Position -= Reference;

			if(dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}

			if(dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Y = rotate(Y, DeltaY, X);
				Z = rotate(Z, DeltaY, X);

				if(Y.y < 0.0f)
				{
					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = cross(Z, X);
				}
			}

			Position = Reference + Z * length(Position);
		}
		
	}
	else
	{
		vec3 vehiclePos = App->player->vehicle->GetPos();
		vec3 vehicleFw = App->player->vehicle->GetFoward();
		distanceFromCar = { 0,3.5,-7.5f };

		mat4x4 transform;

		App->player->vehicle->GetTransform(&transform);

		mat3x3 rotation(transform);

		if (lookAtBall)
		{
			vec3 dir = vehiclePos -App->physics->matchBall->GetPos();

			if (length(dir) < 10)
			{
				if(multiplier < 1.5f)
				multiplier += 0.0125f;
			}
			else
			{
				if(multiplier > 1.0f)
				multiplier -= 0.025f;
			}

			////Foward Vehicle Vector
			dir = normalize(dir);

			//vehiclePos = normalize(vehiclePos);

			//btVector3 fw = { vehiclePos.x, vehiclePos.y, vehiclePos.z };
			//btVector3 dir2 = { dir.x, dir.y, dir.z };
			//float angle = btAngle(dir2, fw);

			//mat3x3 newrot(vec3(cos(angle), 0, sin(angle)), vec3(0, 1, 0), vec3(-sin(angle), 0, cos(angle)));

			//rotation2 = newrot,

			//Position = App->player->vehicle->GetPos() + rotation2 * (rotation * distanceFromCar);

			dir.Set(dir.x,0.f,dir.z);

			newpos = vehiclePos + dir * -distanceFromCar.z * multiplier + vec3(0, distanceFromCar.y, 0);

			btVector3 interpolVec = lerp({ Position.x, Position.y, Position.z }, { newpos.x, newpos.y, newpos.z }, 0.15);

			Position = { interpolVec.x() , interpolVec.y() , interpolVec.z() };

			LookAt(App->physics->matchBall->GetPos());
		}
		else
		{
			newpos = vehiclePos + rotation * distanceFromCar;

			btVector3 interpolVec = lerp({Position.x, Position.y, Position.z}, { newpos.x, newpos.y, newpos.z }, 0.15);

			Position = { interpolVec.x() , interpolVec.y() , interpolVec.z() };

			LookAt(vehiclePos + vec3(0, distanceFromCar.y, 0));
		}
	}


	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}