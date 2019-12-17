#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"

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
	mtBall->SetPos(0, 12, 0);

	primitives.PushBack(mtBall);
	mtBall->color.Set(175.f / 255.f, 175.f / 255.f, 175.f / 255.f);

	mtBall->body = App->physics->matchBall =  App->physics->AddBody(*mtBall, 6.f);

	//Field ground---------------------------------------------
	Cube* ground = new Cube(400,10,440);

	ground->color.Set(106.f / 255.f,70.f / 255.f,42.f /255.f);
	ground->SetPos(0, 0, 0);

	primitives.PushBack(ground);

	App->physics->AddBody(*ground,0, CNT_MAP);


	//Field walls----------------------------------------------
	Cube* wallr = new Cube(10, 50, 400);
	wallr->SetPos(-100, 25, 0);

	primitives.PushBack(wallr);
	App->physics->AddBody(*wallr, 0, CNT_MAP);


	Cube* walll = new Cube(10, 50, 400);
	walll->SetPos(100, 25, 0);

	primitives.PushBack(walll);
	App->physics->AddBody(*walll, 0, CNT_MAP);


	Cube* wallfr = new Cube(115, 50, 10);
	wallfr->color.Set(255.f / 255.f, 79.f / 255.f, 1.f / 255.f);
	wallfr->SetPos(-85, 25, 200);

	primitives.PushBack(wallfr);
	App->physics->AddBody(*wallfr, 0, CNT_MAP);

	Cube* wallfl = new Cube(115, 50, 10);
	wallfl->color.Set(255.f / 255.f, 79.f / 255.f, 1.f / 255.f);
	wallfl->SetPos(85, 25, 200);

	primitives.PushBack(wallfl);
	App->physics->AddBody(*wallfl, 0, CNT_MAP);

	Cube* wallft = new Cube(60, 50, 10);
	wallft->color.Set(255.f / 255.f, 79.f / 255.f, 1.f / 255.f);
	wallft->SetPos(0, 45, 200);

	primitives.PushBack(wallft);
	App->physics->AddBody(*wallft, 0, CNT_MAP);



	Cube* wallbr = new Cube(115, 50, 10);
	wallbr->color.Set(0.f / 255.f, 73.f / 255.f, 255.f / 255.f);
	wallbr->SetPos(-85, 25, -200);

	primitives.PushBack(wallbr);
	App->physics->AddBody(*wallbr, 0, CNT_MAP);


	Cube* wallbl = new Cube(115, 50, 10);
	wallbl->color.Set(0.f / 255.f, 73.f / 255.f, 255.f / 255.f);
	wallbl->SetPos(85, 25, -200);

	primitives.PushBack(wallbl);
	App->physics->AddBody(*wallbl, 0, CNT_MAP);

	Cube* wallbt = new Cube(60, 50, 10);
	wallbt->color.Set(0.f / 255.f, 73.f / 255.f, 255.f / 255.f);
	wallbt->SetPos(0, 45, -200);

	primitives.PushBack(wallbt);
	App->physics->AddBody(*wallbt, 0, CNT_MAP);

	//Field celling---------------------------------------------
	Cube* cell = new Cube(400, 5, 400);

	cell->color.Set(0.f / 255.f, 255.f / 255.f, 234.f / 255.f);
	cell->SetPos(0, 50, 0);

	primitives.PushBack(cell);

	App->physics->AddBody(*cell, 0, CNT_MAP);

	//Goals ----------------------------------------------------
	//Blue
	{
		Cube* goal_blue_top = new Cube(55, 1.5f, 20);
		goal_blue_top->color.Set(200.f / 255.f, 200.f / 255.f, 200.f / 255.f);
		goal_blue_top->SetPos(0, 19.25f, -205);

		primitives.PushBack(goal_blue_top);
		App->physics->AddBody(*goal_blue_top, 0);


		Cube* goal_blue_left = new Cube(3, 15, 20);
		goal_blue_left->color.Set(200.f / 255.f, 200.f / 255.f, 200.f / 255.f);
		goal_blue_left->SetPos(-26, 12.5f, -205);

		primitives.PushBack(goal_blue_left);
		App->physics->AddBody(*goal_blue_left, 0);


		Cube* goal_blue_right = new Cube(3, 15, 20);
		goal_blue_right->color.Set(200.f / 255.f, 200.f / 255.f, 200.f / 255.f);
		goal_blue_right->SetPos(26, 12.5f, -205);

		primitives.PushBack(goal_blue_right);
		App->physics->AddBody(*goal_blue_right, 0);


		Cube* goal_blue_back = new Cube(55, 15, 1.5f);
		goal_blue_back->color.Set(0.f / 255.f, 143.f / 255.f, 255.f / 255.f);
		goal_blue_back->SetPos(0, 12.5f, -215);

		primitives.PushBack(goal_blue_back);
		App->physics->AddBody(*goal_blue_back, 0);


		Cube* goal_blue_sensor = new Cube(49, 13.5f, 20);
		goal_blue_sensor->color.Set(0.f / 255.f, 143.f / 255.f, 255.f / 255.f);
		goal_blue_sensor->SetPos(0, 11.75f, -205);

		primitives.PushBack(goal_blue_sensor);
		goal_blue_sensor->body = App->physics->AddBody(*goal_blue_sensor, 0, CNT_BLUE_GOAL);
		goal_blue_sensor->body->SetAsSensor(true);
		goal_blue_sensor->body->collision_listeners.add(this);
		goal_blue_sensor->SetInvisible(true);
	}

	//Orange
	{
		Cube* goal_orange_top = new Cube(55, 1.5f, 20);
		goal_orange_top->color.Set(200.f / 255.f, 200.f / 255.f, 200.f / 255.f);
		goal_orange_top->SetPos(0, 19.25f, 205);

		primitives.PushBack(goal_orange_top);
		App->physics->AddBody(*goal_orange_top, 0);


		Cube* goal_orange_left = new Cube(3, 15, 20);
		goal_orange_left->color.Set(200.f / 255.f, 200.f / 255.f, 200.f / 255.f);
		goal_orange_left->SetPos(-26, 12.5f, 205);

		primitives.PushBack(goal_orange_left);
		App->physics->AddBody(*goal_orange_left, 0);


		Cube* goal_orange_right = new Cube(3, 15, 20);
		goal_orange_right->color.Set(200.f / 255.f, 200.f / 255.f, 200.f / 255.f);
		goal_orange_right->SetPos(26, 12.5f, 205);

		primitives.PushBack(goal_orange_right);
		App->physics->AddBody(*goal_orange_right, 0);


		Cube* goal_orange_back = new Cube(55, 15, 1.5f);
		goal_orange_back->color.Set(255.f / 255.f, 149.f / 255.f, 1.f / 255.f);
		goal_orange_back->SetPos(0, 12.5f, 215);

		primitives.PushBack(goal_orange_back);
		App->physics->AddBody(*goal_orange_back, 0);


		Cube* goal_orange_sensor = new Cube(49, 13.5f, 20);
		goal_orange_sensor->color.Set(0.f / 255.f, 143.f / 255.f, 255.f / 255.f);
		goal_orange_sensor->SetPos(0, 11.75f, 205);

		primitives.PushBack(goal_orange_sensor);
		goal_orange_sensor->body = App->physics->AddBody(*goal_orange_sensor, 0, CNT_ORANGE_GOAL);
		goal_orange_sensor->body->SetAsSensor(true);
		goal_orange_sensor->body->collision_listeners.add(this);
		goal_orange_sensor->SetInvisible(true);
	}

	//Big Boosters ---------------------------------------------

	//Mid-----
	{
		Cube* bigPag_mid_left = new Cube(2, 1.25f, 2);

		bigPag_mid_left->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		bigPag_mid_left->SetPos(-80, 6.5f, 0);

		primitives.PushBack(bigPag_mid_left);

		bigPag_mid_left->body = App->physics->AddBody(*bigPag_mid_left, 0, CNT_BIG_BOOST);
		bigPag_mid_left->body->SetAsSensor(true);
		bigPag_mid_left->body->collision_listeners.add(this);


		Cube* bigPag_mid_right = new Cube(2, 1.25f, 2);

		bigPag_mid_right->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		bigPag_mid_right->SetPos(80, 6.5f, 0);

		primitives.PushBack(bigPag_mid_right);

		bigPag_mid_right->body = App->physics->AddBody(*bigPag_mid_right, 0, CNT_BIG_BOOST);
		bigPag_mid_right->body->SetAsSensor(true);
		bigPag_mid_right->body->collision_listeners.add(this);
	}


	//Back-------
	{
		Cylinder* bigPag_back_left = new Cylinder(3.5f, 1.5f);

		bigPag_back_left->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		bigPag_back_left->SetPos(-70, 6.5f, -160);

		primitives.PushBack(bigPag_back_left);

		bigPag_back_left->body = App->physics->AddBody(*bigPag_back_left, 0, CNT_BIG_BOOST);
		bigPag_back_left->body->SetAsSensor(true);

		mat4x4 trans;
		bigPag_back_left->body->GetTransform(&trans);
		trans.rotate(90, { 0,0,1 });
		bigPag_back_left->body->SetTransform(&trans);

		bigPag_back_left->body->collision_listeners.add(this);


		Cylinder* bigPag_back_right = new Cylinder(3.5f, 1.5f);

		bigPag_back_right->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		bigPag_back_right->SetPos(70, 6.5f, -160);

		primitives.PushBack(bigPag_back_right);

		bigPag_back_right->body = App->physics->AddBody(*bigPag_back_right, 0, CNT_BIG_BOOST);
		bigPag_back_right->body->SetAsSensor(true);

		bigPag_back_right->body->GetTransform(&trans);
		trans.rotate(90, { 0,0,1 });
		bigPag_back_right->body->SetTransform(&trans);

		bigPag_back_right->body->collision_listeners.add(this);
	}

	//Front-------
	{
		Cube* bigPag_front_left = new Cube(3, 1.25f, 3);

		bigPag_front_left->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		bigPag_front_left->SetPos(-70, 6.5f, 160);

		primitives.PushBack(bigPag_front_left);

		bigPag_front_left->body = App->physics->AddBody(*bigPag_front_left, 0, CNT_BIG_BOOST);
		bigPag_front_left->body->SetAsSensor(true);
		bigPag_front_left->body->collision_listeners.add(this);


		Cube* bigPag_front_right = new Cube(3, 1.25f, 3);

		bigPag_front_right->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		bigPag_front_right->SetPos(70, 6.5f, 160);

		primitives.PushBack(bigPag_front_right);

		bigPag_front_right->body = App->physics->AddBody(*bigPag_front_right, 0, CNT_BIG_BOOST);
		bigPag_front_right->body->SetAsSensor(true);
		bigPag_front_right->body->collision_listeners.add(this);
	}


	//Little Boosters ---------------------------------------------
	//Back Middle
	{
		Cube* little_Pag_back_left = new Cube(1, 0.25f, 1);

		little_Pag_back_left->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_back_left->SetPos(-30, 6.5f, -175);

		primitives.PushBack(little_Pag_back_left);

		little_Pag_back_left->body = App->physics->AddBody(*little_Pag_back_left, 0, CNT_LITTLE_BOOST);
		little_Pag_back_left->body->SetAsSensor(true);
		little_Pag_back_left->body->collision_listeners.add(this);


		Cube* little_Pag_back_mid = new Cube(1, 0.25f, 1);

		little_Pag_back_mid->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_back_mid->SetPos(0, 6.5f, -175);

		primitives.PushBack(little_Pag_back_mid);

		little_Pag_back_mid->body = App->physics->AddBody(*little_Pag_back_mid, 0, CNT_LITTLE_BOOST);
		little_Pag_back_mid->body->SetAsSensor(true);
		little_Pag_back_mid->body->collision_listeners.add(this);


		Cube* little_Pag_back_right = new Cube(1, 0.25f, 1);

		little_Pag_back_right->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_back_right->SetPos(30, 6.5f, -175);

		primitives.PushBack(little_Pag_back_right);

		little_Pag_back_right->body = App->physics->AddBody(*little_Pag_back_right, 0, CNT_LITTLE_BOOST);
		little_Pag_back_right->body->SetAsSensor(true);
		little_Pag_back_right->body->collision_listeners.add(this);
	}

	//Front Middle
	{
		Cube* little_Pag_front_left = new Cube(1, 0.25f, 1);

		little_Pag_front_left->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_front_left->SetPos(-30, 6.5f, 175);

		primitives.PushBack(little_Pag_front_left);

		little_Pag_front_left->body = App->physics->AddBody(*little_Pag_front_left, 0, CNT_LITTLE_BOOST);
		little_Pag_front_left->body->SetAsSensor(true);
		little_Pag_front_left->body->collision_listeners.add(this);


		Cube* little_Pag_front_mid = new Cube(1, 0.25f, 1);

		little_Pag_front_mid->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_front_mid->SetPos(0, 6.5f, 175);

		primitives.PushBack(little_Pag_front_mid);

		little_Pag_front_mid->body = App->physics->AddBody(*little_Pag_front_mid, 0, CNT_LITTLE_BOOST);
		little_Pag_front_mid->body->SetAsSensor(true);
		little_Pag_front_mid->body->collision_listeners.add(this);


		Cube* little_Pag_front_right = new Cube(1, 0.25f, 1);

		little_Pag_front_right->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_front_right->SetPos(30, 6.5f, 175);

		primitives.PushBack(little_Pag_front_right);

		little_Pag_front_right->body = App->physics->AddBody(*little_Pag_front_right, 0, CNT_LITTLE_BOOST);
		little_Pag_front_right->body->SetAsSensor(true);
		little_Pag_front_right->body->collision_listeners.add(this);
	}

	//Right
	{
		Cube* little_Pag_right_front = new Cube(1, 0.25f, 1);

		little_Pag_right_front->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_right_front->SetPos(70, 6.5f, 85);

		primitives.PushBack(little_Pag_right_front);

		little_Pag_right_front->body = App->physics->AddBody(*little_Pag_right_front, 0, CNT_LITTLE_BOOST);
		little_Pag_right_front->body->SetAsSensor(true);
		little_Pag_right_front->body->collision_listeners.add(this);


		Cube* little_Pag_right_back = new Cube(1, 0.25f, 1);

		little_Pag_right_back->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_right_back->SetPos(70, 6.5f, -85);

		primitives.PushBack(little_Pag_right_back);

		little_Pag_right_back->body = App->physics->AddBody(*little_Pag_right_back, 0, CNT_LITTLE_BOOST);
		little_Pag_right_back->body->SetAsSensor(true);
		little_Pag_right_back->body->collision_listeners.add(this);

	}

	//Left
	{
		Cube* little_Pag_left_front = new Cube(1, 0.25f, 1);

		little_Pag_left_front->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_left_front->SetPos(-70, 6.5f, 85);

		primitives.PushBack(little_Pag_left_front);

		little_Pag_left_front->body = App->physics->AddBody(*little_Pag_left_front, 0, CNT_LITTLE_BOOST);
		little_Pag_left_front->body->SetAsSensor(true);
		little_Pag_left_front->body->collision_listeners.add(this);


		Cube* little_Pag_left_back = new Cube(1, 0.25f, 1);

		little_Pag_left_back->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_left_back->SetPos(-70, 6.5f, -85);

		primitives.PushBack(little_Pag_left_back);

		little_Pag_left_back->body = App->physics->AddBody(*little_Pag_left_back, 0, CNT_LITTLE_BOOST);
		little_Pag_left_back->body->SetAsSensor(true);
		little_Pag_left_back->body->collision_listeners.add(this);

	}


	//Center
	{
		Cube* little_Pag_center_left = new Cube(1, 0.25f, 1);

		little_Pag_center_left->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_center_left->SetPos(-20, 6.5f, 0);

		primitives.PushBack(little_Pag_center_left);

		little_Pag_center_left->body = App->physics->AddBody(*little_Pag_center_left, 0, CNT_LITTLE_BOOST);
		little_Pag_center_left->body->SetAsSensor(true);
		little_Pag_center_left->body->collision_listeners.add(this);


		Cube* little_Pag_center_right = new Cube(1, 0.25f, 1);

		little_Pag_center_right->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_center_right->SetPos(20, 6.5f, 0);

		primitives.PushBack(little_Pag_center_right);

		little_Pag_center_right->body = App->physics->AddBody(*little_Pag_center_right, 0, CNT_LITTLE_BOOST);
		little_Pag_center_right->body->SetAsSensor(true);
		little_Pag_center_right->body->collision_listeners.add(this);


		Cube* little_Pag_center_back = new Cube(1, 0.25f, 1);

		little_Pag_center_back->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_center_back->SetPos(0, 6.5f, -20);

		primitives.PushBack(little_Pag_center_back);

		little_Pag_center_back->body = App->physics->AddBody(*little_Pag_center_back, 0, CNT_LITTLE_BOOST);
		little_Pag_center_back->body->SetAsSensor(true);
		little_Pag_center_back->body->collision_listeners.add(this);


		Cube* little_Pag_center_front = new Cube(1, 0.25f, 1);

		little_Pag_center_front->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_center_front->SetPos(0, 6.5f, 20);

		primitives.PushBack(little_Pag_center_front);

		little_Pag_center_front->body = App->physics->AddBody(*little_Pag_center_front, 0, CNT_LITTLE_BOOST);
		little_Pag_center_front->body->SetAsSensor(true);
		little_Pag_center_front->body->collision_listeners.add(this);

	}

	//Mid-Field
	{
		Cube* little_Pag_CBack_left = new Cube(1, 0.25f, 1);

		little_Pag_CBack_left->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_CBack_left->SetPos(-30, 6.5f, -30);

		primitives.PushBack(little_Pag_CBack_left);

		little_Pag_CBack_left->body = App->physics->AddBody(*little_Pag_CBack_left, 0, CNT_LITTLE_BOOST);
		little_Pag_CBack_left->body->SetAsSensor(true);
		little_Pag_CBack_left->body->collision_listeners.add(this);


		Cube* little_Pag_CBack_right = new Cube(1, 0.25f, 1);

		little_Pag_CBack_right->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_CBack_right->SetPos(30, 6.5f, -30);

		primitives.PushBack(little_Pag_CBack_right);

		little_Pag_CBack_right->body = App->physics->AddBody(*little_Pag_CBack_right, 0, CNT_LITTLE_BOOST);
		little_Pag_CBack_right->body->SetAsSensor(true);
		little_Pag_CBack_right->body->collision_listeners.add(this);


		Cube* little_Pag_CFront_left = new Cube(1, 0.25f, 1);

		little_Pag_CFront_left->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_CFront_left->SetPos(-30, 6.5f, 30);

		primitives.PushBack(little_Pag_CFront_left);

		little_Pag_CFront_left->body = App->physics->AddBody(*little_Pag_CFront_left, 0, CNT_LITTLE_BOOST);
		little_Pag_CFront_left->body->SetAsSensor(true);
		little_Pag_CFront_left->body->collision_listeners.add(this);


		Cube* little_Pag_CFront_right = new Cube(1, 0.25f, 1);

		little_Pag_CFront_right->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_CFront_right->SetPos(30, 6.5f, 30);

		primitives.PushBack(little_Pag_CFront_right);

		little_Pag_CFront_right->body = App->physics->AddBody(*little_Pag_CFront_right, 0, CNT_LITTLE_BOOST);
		little_Pag_CFront_right->body->SetAsSensor(true);
		little_Pag_CFront_right->body->collision_listeners.add(this);

	}

	//Between Goal-Mid Field (Blue Side)
	{
		Cube* little_Pag_GMB_Left = new Cube(1, 0.25f, 1);

		little_Pag_GMB_Left->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_GMB_Left->SetPos(-25, 6.5f, -60);

		primitives.PushBack(little_Pag_GMB_Left);

		little_Pag_GMB_Left->body = App->physics->AddBody(*little_Pag_GMB_Left, 0, CNT_LITTLE_BOOST);
		little_Pag_GMB_Left->body->SetAsSensor(true);
		little_Pag_GMB_Left->body->collision_listeners.add(this);


		Cube* little_Pag_GMB_mid_left = new Cube(1, 0.25f, 1);

		little_Pag_GMB_mid_left->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_GMB_mid_left->SetPos(-15, 6.5f, -100);

		primitives.PushBack(little_Pag_GMB_mid_left);

		little_Pag_GMB_mid_left->body = App->physics->AddBody(*little_Pag_GMB_mid_left, 0, CNT_LITTLE_BOOST);
		little_Pag_GMB_mid_left->body->SetAsSensor(true);
		little_Pag_GMB_mid_left->body->collision_listeners.add(this);


		Cube* little_Pag_GMB_mid = new Cube(1, 0.25f, 1);

		little_Pag_GMB_mid->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_GMB_mid->SetPos(0, 6.5f, -130);

		primitives.PushBack(little_Pag_GMB_mid);

		little_Pag_GMB_mid->body = App->physics->AddBody(*little_Pag_GMB_mid, 0, CNT_LITTLE_BOOST);
		little_Pag_GMB_mid->body->SetAsSensor(true);
		little_Pag_GMB_mid->body->collision_listeners.add(this);


		Cube* little_Pag_GMB_mid_right = new Cube(1, 0.25f, 1);

		little_Pag_GMB_mid_right->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_GMB_mid_right->SetPos(15, 6.5f, -100);

		primitives.PushBack(little_Pag_GMB_mid_right);

		little_Pag_GMB_mid_right->body = App->physics->AddBody(*little_Pag_GMB_mid_right, 0, CNT_LITTLE_BOOST);
		little_Pag_GMB_mid_right->body->SetAsSensor(true);
		little_Pag_GMB_mid_right->body->collision_listeners.add(this);


		Cube* little_Pag_GMB_right = new Cube(1, 0.25f, 1);

		little_Pag_GMB_right->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_GMB_right->SetPos(25, 6.5f, -60);

		primitives.PushBack(little_Pag_GMB_right);

		little_Pag_GMB_right->body = App->physics->AddBody(*little_Pag_GMB_right, 0, CNT_LITTLE_BOOST);
		little_Pag_GMB_right->body->SetAsSensor(true);
		little_Pag_GMB_right->body->collision_listeners.add(this);

	}


	//Between Goal-Mid Field (Orange Side)
	{
		Cube* little_Pag_GMO_Left = new Cube(1, 0.25f, 1);

		little_Pag_GMO_Left->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_GMO_Left->SetPos(-25, 6.5f, 60);

		primitives.PushBack(little_Pag_GMO_Left);

		little_Pag_GMO_Left->body = App->physics->AddBody(*little_Pag_GMO_Left, 0, CNT_LITTLE_BOOST);
		little_Pag_GMO_Left->body->SetAsSensor(true);
		little_Pag_GMO_Left->body->collision_listeners.add(this);


		Cube* little_Pag_GMO_mid_left = new Cube(1, 0.25f, 1);

		little_Pag_GMO_mid_left->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_GMO_mid_left->SetPos(-15, 6.5f, 100);

		primitives.PushBack(little_Pag_GMO_mid_left);

		little_Pag_GMO_mid_left->body = App->physics->AddBody(*little_Pag_GMO_mid_left, 0, CNT_LITTLE_BOOST);
		little_Pag_GMO_mid_left->body->SetAsSensor(true);
		little_Pag_GMO_mid_left->body->collision_listeners.add(this);


		Cube* little_Pag_GMO_mid = new Cube(1, 0.25f, 1);

		little_Pag_GMO_mid->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_GMO_mid->SetPos(0, 6.5f, 130);

		primitives.PushBack(little_Pag_GMO_mid);

		little_Pag_GMO_mid->body = App->physics->AddBody(*little_Pag_GMO_mid, 0, CNT_LITTLE_BOOST);
		little_Pag_GMO_mid->body->SetAsSensor(true);
		little_Pag_GMO_mid->body->collision_listeners.add(this);


		Cube* little_Pag_GMO_mid_right = new Cube(1, 0.25f, 1);

		little_Pag_GMO_mid_right->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_GMO_mid_right->SetPos(15, 6.5f, 100);

		primitives.PushBack(little_Pag_GMO_mid_right);

		little_Pag_GMO_mid_right->body = App->physics->AddBody(*little_Pag_GMO_mid_right, 0, CNT_LITTLE_BOOST);
		little_Pag_GMO_mid_right->body->SetAsSensor(true);
		little_Pag_GMO_mid_right->body->collision_listeners.add(this);


		Cube* little_Pag_GMO_right = new Cube(1, 0.25f, 1);

		little_Pag_GMO_right->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
		little_Pag_GMO_right->SetPos(25, 6.5f, 60);

		primitives.PushBack(little_Pag_GMO_right);

		little_Pag_GMO_right->body = App->physics->AddBody(*little_Pag_GMO_right, 0, CNT_LITTLE_BOOST);
		little_Pag_GMO_right->body->SetAsSensor(true);
		little_Pag_GMO_right->body->collision_listeners.add(this);

	}


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

	return UPDATE_CONTINUE;
}

bool ModuleSceneIntro::Draw()
{
	for (uint n = 0; n < primitives.Count(); n++)
	{
		if(!primitives[n]->isInvisible)
		primitives[n]->Render();
	}

	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	return true;
}
// Update
update_status ModuleSceneIntro::Update(float dt)
{
	for (uint n = 0; n < primitives.Count(); n++)
	{
		if (primitives[n]->body != nullptr)
		{
			primitives[n]->Update();

			PhysBody3D* body =  primitives[n]->body;

			if (body->is_sensor && !body->sensorOnline)
			{
				if (primitives[n]->body->innerSensorTimer == 0.f)
					primitives[n]->color.Set(0.7f, 0.7f, 0.7f);

				primitives[n]->body->innerSensorTimer += dt;

				if (primitives[n]->body->innerSensorTimer > 3.f)
				{
					primitives[n]->body->sensorOnline = true;
					primitives[n]->color.Set(247.f / 255.f, 240.f / 255.f, 62.f / 255.f);
					primitives[n]->body->innerSensorTimer = 0.f;

				}
			}
		}
	}



	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	switch (body1->cntType)
	{
	case CNT_BIG_BOOST:
		if (body2->cntType == CNT_VEHICLE)
		{
			//TODO: Add trubo here
			body1->sensorOnline = false;
		}
		break;

	case CNT_LITTLE_BOOST:
		if (body2->cntType == CNT_VEHICLE)
		{
			//TODO: Add trubo here
			body1->sensorOnline = false;
		}
		break;

	case CNT_BLUE_GOAL:

		break;

	case CNT_ORANGE_GOAL:

		break;

	}

}

