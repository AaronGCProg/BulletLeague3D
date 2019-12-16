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

	//Field ground---------------------------------------------
	Cube* ground = new Cube(400,10,400);

	ground->color.Set(106.f / 255.f,70.f / 255.f,42.f /255.f);
	ground->SetPos(0, 0, 0);

	primitives.PushBack(ground);

	 App->physics->AddBody(*ground,0, CNT_GROUND);


	//Field walls----------------------------------------------
	Cube* wallr = new Cube(10, 50, 400);
	wallr->SetPos(-100, 25, 0);

	primitives.PushBack(wallr);
	App->physics->AddBody(*wallr, 0);


	Cube* walll = new Cube(10, 50, 400);
	walll->SetPos(100, 25, 0);

	primitives.PushBack(walll);
	App->physics->AddBody(*walll, 0);


	Cube* wallf = new Cube(400, 50, 10);
	wallf->color.Set(255.f / 255.f, 79.f / 255.f, 1.f / 255.f);
	wallf->SetPos(0, 25, 200);

	primitives.PushBack(wallf);
	App->physics->AddBody(*wallf, 0);



	Cube* wallb = new Cube(400, 50, 10);
	wallb->color.Set(0.f / 255.f, 73.f / 255.f, 255.f / 255.f);
	wallb->SetPos(0, 25, -200);

	primitives.PushBack(wallb);
	App->physics->AddBody(*wallb, 0);

	//Field celling---------------------------------------------
	Cube* cell = new Cube(400, 10, 400);

	cell->color.Set(0.f / 255.f, 255.f / 255.f, 234.f / 255.f);
	cell->SetPos(0, 50, 0);

	primitives.PushBack(cell);

	App->physics->AddBody(*cell, 0);


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

