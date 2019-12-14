#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");

	//Create MatchBall------------------------------------
	Sphere* mtBall = new Sphere(3);
	mtBall->SetPos(0, 12, 30);
	float force = 0.0f;

	primitives.PushBack(mtBall);
	mtBall->color.Set(175.f / 255.f, 175.f / 255.f, 175.f / 255.f);

	mtBall->body = App->physics->AddBody(*mtBall, 0.3);


	Cube* ground = new Cube(400,1,400);

	ground->color.Set(106.f / 255.f,70.f / 255.f,42.f /255.f);
	ground->SetPos(0, 0, 0);

	primitives.PushBack(ground);

	App->physics->AddBody(*ground,0);



	Cube wall1(1, 100, 1);
	wall1.SetPos(0, 0, 0);

	App->physics->AddBody(wall1, 0);


	bool ret = true;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	for (int n = 0; n < primitives.Count(); n++)
	{
		primitives[n]->body = nullptr;
		delete primitives[n];
	}
	primitives.Clear();


	return true;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	//TODO 3: Nothing to do here. But it's good to know where all primitives are being rendered
	for (uint n = 0; n < primitives.Count(); n++)
	{
		primitives[n]->Render();
	}

	return UPDATE_CONTINUE;
}
// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	for (uint n = 0; n < primitives.Count(); n++)
	{
		if(primitives[n]->body != nullptr)
		primitives[n]->Update();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

