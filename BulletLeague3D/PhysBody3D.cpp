#include "PhysBody3D.h"
#include "glmath.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// =================================================
PhysBody3D::PhysBody3D(btRigidBody* body) : body(body), cntType(CNT_UNKNOWN), innerSensorTimer(0.0f), is_sensor(false)
{}

PhysBody3D::PhysBody3D(btRigidBody* body, ContactType type) : body(body), cntType(type), innerSensorTimer(0.0f), is_sensor(false)
{}

// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	delete body;
}

// ---------------------------------------------------------
void PhysBody3D::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		body->getWorldTransform().getOpenGLMatrix(matrix);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
}


// ---------------------------------------------------------
vec3 PhysBody3D::GetPos() const
{
	btTransform t = body->getWorldTransform();

	btVector3 &position = t.getOrigin();

	vec3 Vpos = { position.x(), position.y(), position.z() };

	return Vpos;
}

// ---------------------------------------------------------
vec3 PhysBody3D::GetFoward() 
{
	mat4x4 transform;
	GetTransform(&transform);


	mat3x3 rotation(transform);

	vec3 fw(1.f,1.f, 1.f);

	fw = rotation * fw;


	return fw;
}

// ---------------------------------------------------------
void PhysBody3D::SetAsSensor(bool is_sensor)
{
	if (this->is_sensor != is_sensor)
	{
		this->is_sensor = is_sensor;
		if (is_sensor == true)
		{
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}
		else
			body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetRestitution(btScalar restitution)
{
	this->body->setRestitution(restitution);
}

