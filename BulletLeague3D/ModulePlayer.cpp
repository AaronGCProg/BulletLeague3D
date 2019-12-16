#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModuleSceneIntro.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	

	// Car properties ----------------------------------------

	car.sensor = new Cube(0.2f, 0.2f, 0.2f);
	App->physics->AddBody(*car.sensor, 0, CNT_GROUND)->SetAsSensor(true);
	App->scene_intro->primitives.PushBack(car.sensor);


	//All chassis parts
	car.num_chassis = 5;
	car.chassis = new Chassis[car.num_chassis];

	//front mudward
	car.chassis[0].chassis_size.Set(2, 0.5, 1);
	car.chassis[0].chassis_offset.Set(0, 1.f, 2.5f);

	//back mudward
	car.chassis[1].chassis_size.Set(2, 0.5, 1);
	car.chassis[1].chassis_offset.Set(0, 1.f, -2.5f);

	//spoiler
	car.chassis[2].chassis_size.Set(0.1f, 0.6f, 0.2f);
	car.chassis[2].chassis_offset.Set(-0.5, 1.6f, -2.75f);

	car.chassis[3].chassis_size.Set(0.1f, 0.6f, 0.2f);
	car.chassis[3].chassis_offset.Set(0.5, 1.6f, -2.75f);

	car.chassis[4].chassis_size.Set(2.f, 0.2f, 0.4f);
	car.chassis[4].chassis_offset.Set(0.f, 2.f, -2.75f);


	car.chassis_size.Set(2, 1, 4);
	car.chassis_offset.Set(0, 1, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 0.8f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x * 0.5f;
	float half_length = car.chassis_size.z * 0.5f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	car.num_wheels = 4;
	car.wheels = new Wheel[car.num_wheels];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.4f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	jumpImpulse = false;
	canDrift = false;
	secondJump = false;

	

	vehicle = App->physics->AddVehicle(car);
	vehicle->collision_listeners.add(this);
	vehicle->cntType = CNT_VEHICLE;
	vehicle->SetPos(0, 8, -170);



	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	car.sensor->SetPos(vehicle->GetPos().x, vehicle->GetPos().y, vehicle->GetPos().z);


	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && wallContact[CNT_GROUND])
	{
		acceleration = MAX_ACCELERATION;		

		vehicle->Push(0.0f, -STICK_FORCE, 0.0f);
	}
	else if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && !wallContact[CNT_GROUND])
	{
	
		if(vehicle->myBody->getAngularVelocity().getX() < MAX_ACROBATIC_SPEED)
			vehicle->myBody->applyTorque(WorldToLocal(5000.0f, 0.0f, 0.0f));
	}

	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && wallContact[CNT_GROUND])
	{
		if(turn < TURN_DEGREES && !canDrift)
			turn +=  TURN_DEGREES;
		else if (turn > -TURN_DEGREES && canDrift) 
		{
			turn += TURN_DEGREES;
			
			vehicle->myBody->applyTorque(WorldToLocal(0.0f, 10000.0f, 0.0f));
		}

		vehicle->Push(0.0f, -STICK_FORCE, 0.0f);
	
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !wallContact[CNT_GROUND])
	{

		if (secondJump) 
		{
			if (vehicle->myBody->getAngularVelocity().getZ() > -MAX_ACROBATIC_SPEED)
				vehicle->myBody->applyTorque(WorldToLocal(0.0f, 0.0f, -500.0f));
		
		}
		else 
		{

			if (vehicle->myBody->getAngularVelocity().getY() < MAX_ACROBATIC_SPEED)
				vehicle->myBody->applyTorque(WorldToLocal(0.0f, 5000.0f, 0.0f));
		}
			
	}

	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && wallContact[CNT_GROUND])
	{
		if(turn > -TURN_DEGREES && !canDrift)
			turn -= TURN_DEGREES;
		else if (turn > -TURN_DEGREES && canDrift) 
		{
			turn -= TURN_DEGREES;
			
			vehicle->myBody->applyTorque(WorldToLocal(0.0f, -10000.0f, 0.0f));
		}
			
		vehicle->Push(0.0f, -STICK_FORCE, 0.0f);
	
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !wallContact[CNT_GROUND])
	{

		if (secondJump) 
		{

			if (vehicle->myBody->getAngularVelocity().getZ() < MAX_ACROBATIC_SPEED)
				vehicle->myBody->applyTorque(WorldToLocal(0.0f, 0.0f, 500.0f));
		}			
		else 
		{
		
			if (vehicle->myBody->getAngularVelocity().getY() > -MAX_ACROBATIC_SPEED)
				vehicle->myBody->applyTorque(WorldToLocal(0.0f, -5000.0f, 0.0f));

		}
		
	}

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && wallContact[CNT_GROUND])
	{
		acceleration = -MAX_ACCELERATION ;

		vehicle->Push(0.0f, -STICK_FORCE, 0.0f);
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && !wallContact[CNT_GROUND])
	{
		if (vehicle->myBody->getAngularVelocity().getX() > -MAX_ACROBATIC_SPEED)
			vehicle->myBody->applyTorque(WorldToLocal(-5000.0f, 0.0f, 0.0f));
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && wallContact[CNT_GROUND])
	{
		vehicle->myBody->setAngularVelocity({ 0,0,0 });
		vehicle->Push(0.0f, JUMP_FORCE, 0.0f);	
		wallContact[CNT_GROUND] = false;
		
	}
	else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !wallContact[CNT_GROUND] && !secondJump)
	{			
		secondJump = true;

		vehicle->Push(0.0f, IMPULSE_FORCE, 0.0f);
			
	
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) 
		{
			jumpImpulse = true;
			vehicle->myBody->applyCentralForce(WorldToLocal(0.0f, 0.0f, 300000.0f));
			vehicle->myBody->applyTorque(WorldToLocal(115000.0f, 0.0f, 0.0f));
		}		
		else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) 
		{
			jumpImpulse = true;
			vehicle->myBody->applyCentralForce(WorldToLocal(0.0f, 0.0f, -300000.0f));
			vehicle->myBody->applyTorque(WorldToLocal(-115000.0f, 0.0f, 0.0f));
		}		
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) 
		{
			jumpImpulse = true;
			vehicle->myBody->applyCentralForce(WorldToLocal(-300000.0f, 0.0f, 0.0f));
			vehicle->myBody->applyTorque(WorldToLocal(0.0f, 0.0f, 45000.0f));
		}			
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
		{
			jumpImpulse = true;
			vehicle->myBody->applyCentralForce(WorldToLocal(300000.0f, 0.0f, 0.0f));
			vehicle->myBody->applyTorque(WorldToLocal(0.0f, 0.0f, -45000.0f));
		}
			
	
	}

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
		canDrift = true;

	}
	else if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_UP)
		canDrift = false;


	if (App->input->GetMouseButton(1) == KEY_REPEAT) 
	{
		vehicle->myBody->applyCentralImpulse(WorldToLocal(0.0f, 0.0f, 100.0f));
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	char title[80];
	sprintf_s(title, "%.1f Km/h | Angular Speed %.1f X , %.1f Y, %.1f Z", vehicle->GetKmh(), 
		vehicle->myBody->getAngularVelocity().getX(), vehicle->myBody->getAngularVelocity().getY(), vehicle->myBody->getAngularVelocity().getZ());
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

// World to Local forces translation
btVector3 ModulePlayer::WorldToLocal(float x, float y, float z) 
{
	btVector3 relativeForce = btVector3(x, y, z);
	btMatrix3x3& localRot = vehicle->myBody->getWorldTransform().getBasis();
	btVector3 correctedForce = localRot * relativeForce;

	return correctedForce;
}


void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body2->cntType == CNT_GROUND) 
	{
		wallContact[CNT_GROUND] = true;
		secondJump = false;
		jumpImpulse = false;
	}
		
}

