#pragma once
#include "PhysX.h"
#include "extensions/PxDefaultAllocator.h"

PxCooking* PhysX::s_Cooking;
PhysX* PhysX::p_PhysX;

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxReal chainZ = 10.0f;

void PhysX::Init()
{

    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
    mPvd = PxCreatePvd(*mFoundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), true, mPvd);
    PxInitExtensions(*mPhysics, mPvd);

    PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
    mDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = mDispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    mScene = mPhysics->createScene(sceneDesc);

    PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();
    if (pvdClient)
    {
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }

    auto mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);

    PxRigidStatic* groundPlane = PxCreatePlane(*mPhysics, PxPlane(0, 1, 0, 0), *mMaterial);
    mScene->addActor(*groundPlane);

    mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, PxCookingParams(PxTolerancesScale()));
    s_Cooking = mCooking;


    return;





   /* static PxDefaultErrorCallback gDefaultErrorCallback;
    static PxDefaultAllocator gDefaultAllocatorCallback;
    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);

    mPvd = PxCreatePvd(*mFoundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), true, mPvd);

    PxInitExtensions(*mPhysics, mPvd);

    PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
    mDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = mDispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    mScene = mPhysics->createScene(sceneDesc);


#ifdef _DEBUG 
    mScene->getScenePvdClient()->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
#endif

    PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();
    if (pvdClient)
    {
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }
    auto mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);

    PxRigidStatic* groundPlane = PxCreatePlane(*mPhysics, PxPlane(0, 1, 0, 0), *mMaterial);
    mScene->addActor(*groundPlane);


    bool createCapsule = false;
    if (createCapsule)
    {
        float halfExtent = 5;
        float radius = 0.5;
        auto gMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
        PxShape* shape = mPhysics->createShape(PxCapsuleGeometry(radius, halfExtent), *gMaterial);


        PxTransform t = PxTransform(PxVec3(0, 2, 4));
        PxTransform localTm(PxVec3(0, 0, 0));

        PxRigidDynamic* body = mPhysics->createRigidDynamic(t.transform(localTm));
        body->attachShape(*shape);
        PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
        mScene->addActor(*body);


        m_rigids.push_back(body);
        shape->release();
    } 
    
    bool createSphere = false;
    if (createSphere)
    {
        float radius = 0.5;
        auto gMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
        PxShape* shape = mPhysics->createShape(PxSphereGeometry(radius), *gMaterial);


        PxTransform t = PxTransform(PxVec3(0, 5, 6));
        PxTransform localTm(PxVec3(0, 0, 0));

        PxRigidDynamic* body = mPhysics->createRigidDynamic(t.transform(localTm));
        body->attachShape(*shape);
        PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
        mScene->addActor(*body);


        m_rigids.push_back(body);
        shape->release();
    }
  
    PxReal stackZ = 10.0f;
    float halfExtent = 0.5f;
    float size = 10;
    auto gMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
    PxTransform t = PxTransform(PxVec3(0, 0, stackZ -= 10.0f));

    bool createCubes = false;
    if (createCubes)
    {
        PxShape* shape = mPhysics->createShape(PxBoxGeometry(halfExtent * 0.5, halfExtent, halfExtent * 2), *gMaterial);
        for (PxU32 i = 0; i < size; i++)
        {
            for (PxU32 j = 0; j < size - i; j++)
            {
                PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
                PxRigidDynamic* body = mPhysics->createRigidDynamic(t.transform(localTm));
                body->attachShape(*shape);
                PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
                mScene->addActor(*body);

                m_rigids.push_back(body);
            }
        }
        shape->release();
    }

    */


}

void PhysX::Shutdown()
{
    mPhysics->release(); 
    mFoundation->release();
}

void PhysX::StepPhysics()
{
   /* PxScene* scene;
    const PxPhysics& physX = PxGetPhysics();
    PxGetPhysics().getScenes(&scene, 1);

    PxPhysics::get

    physX.get<SolverComponent>(sceneEntity);

    PxRigidDynamic::getSolverIterationCounts

    mSolver.get
    const auto solver = g.registry.get<SolverComponent>(sceneEntity);
    const float stepsize = time.delta() * solver.timeMultiplier / solver.substeps;
    const double fraction = 1.0f / solver.substeps;

    for (unsigned int i = 1; i <= solver.substeps; i++) {
        updateRigidsSubstep(sceneEntity, fraction * i);

        pxscene->simulate(stepsize);
        pxscene->fetchResults(true);*/
    

    mScene->simulate(1.0f / 60.0f);
    mScene->fetchResults(true);
}


void PhysX::UpdateDrives() 
{
/*    for (auto entity : g.registry.view<SceneComponent, DriveComponent, PxD6Joint*>()) {
        if (g.registry.get<PxD6Joint*>(entity) == nullptr) continue;

        // Update only rigids in this scene
        const auto scene = g.registry.get<SceneComponent>(entity);
        if (scene.entity != sceneEntity) continue;

        const auto drive = g.registry.get<DriveComponent>(entity);
        const auto joint = g.registry.get<JointComponent>(entity);

        // We need this for our hack below, in inverting the child rest
        if (!g.registry.valid(joint.child)) {
            Warning() << "Joint is child-less, this is a bug\n";
            continue;
        }

        auto* pxjoint = g.registry.get<PxD6Joint*>(entity);
        if (drive.enabled) {
            PxD6JointDrive linearDrive{
                drive.linearStiffness,
                drive.linearDamping,

                FLT_MAX,   // Maximum force; ignored
                drive.acceleration,
            };

            pxjoint->setDrive(PxD6Drive::eX, linearDrive);
            pxjoint->setDrive(PxD6Drive::eY, linearDrive);
            pxjoint->setDrive(PxD6Drive::eZ, linearDrive);

            PxD6JointDrive angularDrive{
                drive.angularStiffness,
                drive.angularDamping,

                // Internal defaults
                FLT_MAX,
                drive.acceleration,
            };

            pxjoint->setDrive(PxD6Drive::eTWIST, angularDrive);
            pxjoint->setDrive(PxD6Drive::eSWING, angularDrive);
            pxjoint->setDrive(PxD6Drive::eSLERP, angularDrive);

            // Update target
            //
            // NOTE: Allow for changes to be made to both parent
            // and child frames, without affecting the drive target
            //
            // TODO: Unravel this. We currently can't edit the child anchorpoint
            MMatrix mat = joint.childFrame
                * drive.target
                * joint.parentFrame.inverse();
            // Take negative scale into account
            const auto parentScale = g.registry.get<ScaleComponent>(joint.parent);
            if (parentScale.value.x < 0 || parentScale.value.y < 0 || parentScale.value.z < 0) {
                mat = parentScale.matrix * mat;

                const MTransformationMatrix matTm{ mat };
                const MQuaternion quat = matTm.rotation();
                MTransformationMatrix finalTm{};
                finalTm.setTranslation(matTm.getTranslation(MSpace::kTransform), MSpace::kTransform);
                finalTm.setRotationQuaternion(quat.x, quat.y, quat.z, quat.w);

                mat = finalTm.asMatrix();
            }

            const PxTransform pxtransform{ toPxMat44(mat) };
            pxjoint->setDrivePosition(pxtransform);
        }

        else {
            static const PxD6JointDrive defaultDrive{ 0.0f, 0.0f, 0.0f, false };
            pxjoint->setDrive(PxD6Drive::eX, defaultDrive);
            pxjoint->setDrive(PxD6Drive::eY, defaultDrive);
            pxjoint->setDrive(PxD6Drive::eZ, defaultDrive);
            pxjoint->setDrive(PxD6Drive::eTWIST, defaultDrive);
            pxjoint->setDrive(PxD6Drive::eSWING, defaultDrive);
            pxjoint->setDrive(PxD6Drive::eSLERP, defaultDrive);
        }
    }*/
}

PxPhysics* PhysX::GetPhysics()
{
    return &PxGetPhysics();
}

PxScene* PhysX::GetScene()
{
    PxScene* scene;
    PxGetPhysics().getScenes(&scene, 1);
    return scene;
}

PxMaterial* PhysX::GetDefaultMaterial()
{
    return GetPhysics()->createMaterial(0.5f, 0.5f, 0.6f);
}

PxCooking* PhysX::GetCooking() {
return s_Cooking;
}



PxRigidDynamic* PhysX::createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0))
{
    auto mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
    PxRigidDynamic* dynamic = PxCreateDynamic(*mPhysics, t, geometry, *mMaterial, 10.0f);
    dynamic->setAngularDamping(0.5f);
    dynamic->setLinearVelocity(velocity);
    mScene->addActor(*dynamic);
    return dynamic;
}



PxTriangleMesh* PhysX::CreateBV33TriangleMesh(PxU32 numVertices, const PxVec3* vertices, PxU32 numTriangles, const PxU32* indices,  bool skipMeshCleanup, bool skipEdgeData, bool inserted, bool cookingPerformance, bool meshSizePerfTradeoff)
{
    std::cout << "numVertices: " << numVertices << "\n";
    std::cout << "numTriangles: " << numTriangles << "\n";

    PxTriangleMeshDesc meshDesc;
    meshDesc.points.count = numVertices;
    meshDesc.points.data = vertices;
    meshDesc.points.stride = sizeof(PxVec3);
    meshDesc.triangles.count = numTriangles;
    meshDesc.triangles.data = indices;
    meshDesc.triangles.stride = 3 * sizeof(PxU32);

    PxDefaultMemoryOutputStream writeBuffer;
    PxTriangleMeshCookingResult::Enum result;
    bool status = mCooking->cookTriangleMesh(meshDesc, writeBuffer);
    if (!status)
        std::cout << "FUCKED UP\n";
    else
        std::cout << "MESHED COOKED SUCCESSFULLY\n";


    PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
    
    return mPhysics->createTriangleMesh(readBuffer);
}

void PhysX::SetupCommonCookingParams(PxCookingParams& params, bool skipMeshCleanup, bool skipEdgeData)
{
    // we suppress the triangle mesh remap table computation to gain some speed, as we will not need it 
// in this snippet
    params.suppressTriangleMeshRemapTable = true;

    // If DISABLE_CLEAN_MESH is set, the mesh is not cleaned during the cooking. The input mesh must be valid. 
    // The following conditions are true for a valid triangle mesh :
    //  1. There are no duplicate vertices(within specified vertexWeldTolerance.See PxCookingParams::meshWeldTolerance)
    //  2. There are no large triangles(within specified PxTolerancesScale.)
    // It is recommended to run a separate validation check in debug/checked builds, see below.

    if (!skipMeshCleanup)
        params.meshPreprocessParams &= ~static_cast<PxMeshPreprocessingFlags>(PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH);
    else
        params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;

    // If DISABLE_ACTIVE_EDGES_PREDOCOMPUTE is set, the cooking does not compute the active (convex) edges, and instead 
    // marks all edges as active. This makes cooking faster but can slow down contact generation. This flag may change 
    // the collision behavior, as all edges of the triangle mesh will now be considered active.
    if (!skipEdgeData)
        params.meshPreprocessParams &= ~static_cast<PxMeshPreprocessingFlags>(PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE);
    else
        params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
}