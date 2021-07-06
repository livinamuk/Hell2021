#include "Scene.h"
#include "Helpers/FileImporter.h"

void Scene::Reset()
{
	nurseModel = FileImporter::LoadSkinnedModel("NurseGuy.fbx");  
    
    // Remove any old characters and their ragdolls.
    while (gameCharacters.size() > 0) {
        gameCharacters[0].m_ragdoll.RemovePhysicsObjects();
        gameCharacters.erase(gameCharacters.begin());
    }

    // New test ragdoll
    GameCharacter gameCharacter;
    gameCharacter.m_transform.position = glm::vec3(0, 0.25, -0.5f);
    gameCharacter.m_skinnedModel = FileImporter::LoadSkinnedModel("NurseGuy.fbx");
    gameCharacter.m_ragdoll.BuildFromJsonFile("ragdoll.json", gameCharacter.m_transform);
    gameCharacter.m_skinningMethod = SkinningMethod::RAGDOLL;
    gameCharacters.push_back(gameCharacter);
}

void Scene::NewRagdoll()
{
    GameCharacter gameCharacter;
    gameCharacter.m_transform.position = glm::vec3(0, 5, 0);
    gameCharacter.m_transform.rotation.x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HELL_PI)));
    gameCharacter.m_transform.rotation.y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HELL_PI)));
    gameCharacter.m_transform.rotation.z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HELL_PI)));
    gameCharacter.m_skinnedModel = nurseModel;
    gameCharacter.m_ragdoll.BuildFromJsonFile("ragdoll.json", gameCharacter.m_transform);
    gameCharacter.m_skinningMethod = SkinningMethod::RAGDOLL;
    gameCharacters.push_back(gameCharacter);
}
