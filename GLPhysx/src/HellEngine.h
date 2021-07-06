#pragma once
#include "Header.h"
#include "Core/Scene.h"
#include "Core/CoreGL.h"
#include "Core/Input.h"
#include "Core/Camera.h"
#include "Core/Entity.h"
#include "Helpers/Util.h"
#include "Helpers/FileImporter.h"
#include "Renderer/Decal.h"
#include "Renderer/Model.h"
#include "Renderer/Texture.h"
#include "Renderer/TextBlitter.h"
#include "Renderer/Renderer.h"
#include "Physics/physics.h"
#include "Physics/RaycastResult.h"

#include "Physics/PhysX.h"

class HellEngine
{
public: // methods
	void Init();
	void Update();
	void CheckForKeyPresses();
	void Render();

public: // fields
	PhysX m_physx;
	Renderer m_renderer;
	Scene m_scene;
	Camera m_camera;
	std::unordered_map<std::string, Model> m_models;

	Entity couchEntity;
};