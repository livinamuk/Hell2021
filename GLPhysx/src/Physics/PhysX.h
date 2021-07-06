#pragma once

//#define NDEBUG

#ifdef _DEBUG 
	#define DEBUG
#else
	#define NDEBUG
#endif

#define PVD_HOST "127.0.0.1"
#include "PxPhysicsAPI.h"
#include "Header.h"

using namespace physx;

class PhysX
{
public: // methods
	void Init();
	void Shutdown(); 
	void StepPhysics();

	PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity);
	
	void UpdateDrives();

	PxTriangleMesh* CreateBV33TriangleMesh(PxU32 numVertices, const PxVec3* vertices, PxU32 numTriangles, const PxU32* indices, bool skipMeshCleanup, bool skipEdgeData, bool inserted, bool cookingPerformance, bool meshSizePerfTradeoff);
	void SetupCommonCookingParams(PxCookingParams& params, bool skipMeshCleanup, bool skipEdgeData);

public: // field
	PxFoundation* mFoundation;
	PxPvd* mPvd;
	PxPhysics* mPhysics;
	PxDefaultCpuDispatcher* mDispatcher = NULL;
	PxScene* mScene = NULL;
	PxCooking* mCooking = NULL;

	std::vector<PxRigidDynamic*> m_rigids;


public: // static functions
	static PxPhysics* GetPhysics();
	static PxScene* GetScene();
	static PxMaterial* GetDefaultMaterial();
	static PxCooking* GetCooking();

public: // static variables
	static PhysX* p_PhysX;
	static PxCooking* s_Cooking;
};