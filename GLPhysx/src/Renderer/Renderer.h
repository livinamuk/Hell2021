#pragma once
#include "Renderer/Decal.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Model.h"
#include "Core/Camera.h"
#include "Core/CoreGL.h"
#include "Core/Entity.h"
#include "Physics/physics.h"
#include "Renderer/GBuffer.h"
#include "Physics/Ragdoll.h"
#include "Physics/PhysX.h"
#include "Core/GameCharacter.h"

class Renderer
{
	
public: // functions
	
	void Init();
	void RenderFrame(std::vector<GameCharacter>& gameCharacters, Entity& couch);
	void DrawGrid();

	void RenderFinalImage();
	void TextBlitterPass(Shader* shader);
	void DrawLine(Shader* shader, Line line, glm::mat4 modelMatrix);
	void HotLoadShaders();
	//static void DrawTangentDebugAxis(Shader* shader, glm::mat4 modelMatrix, float lineLength);
	//static void DrawAnimatedEntityDebugBones_Animated(Shader* shader, Ragdoll* ragdoll);
	void DrawPoint(Shader* shader, glm::vec3 position, glm::vec3 color);
	//static void NewGameCharacter();
	//static void RenderJointAxis(Shader*, GameCharacter* gameCharacter);

	void DrawPhysicsWorld();
	void ReconfigureFrameBuffers();
	//Shader* GetShader(const char* name);

public: // variables


	Shader m_test_shader;
	Shader m_solid_color_shader;
	Shader m_textued_2D_quad_shader;
	Shader m_skinned_model_shader;

	GBuffer s_gBuffer;

	PhysX* p_physX;
	std::unordered_map<std::string, Model>* p_models;

	unsigned int m_uboMatrices;
};

