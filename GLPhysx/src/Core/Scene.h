#pragma once
#include "Header.h"
#include "Core/GameCharacter.h"
#include "Animation/SkinnedModel.h"

class Scene
{
public: // methods
	void Reset();
	void NewRagdoll();

public: // fields
	SkinnedModel* nurseModel;
	std::vector<GameCharacter> gameCharacters;

//	std::vector<Entity> m_staticEntities;
};