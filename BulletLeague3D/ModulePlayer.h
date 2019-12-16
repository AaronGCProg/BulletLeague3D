#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysVehicle3D.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 100.0f
#define JUMP_FORCE 4000.0f
#define IMPULSE_FORCE 2000.0f
#define MAX_TORQUE 2000.0f



class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	btVector3 WorldToLocal(float x, float y, float z);
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:

	VehicleInfo car;
	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;

	bool jumpImpulse;
	bool secondJump;
	bool canDrift;

	bool wallContact[CNT_MAX] = { false };

};