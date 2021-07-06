#include "HellEngine.h"
#include "Core/Input.h"

void HellEngine::Init()
{
	CoreGL::InitGL(SCR_WIDTH, SCR_HEIGHT);
	glPointSize(5);

    PhysX::p_PhysX = &m_physx;

	m_physx.Init();
	m_renderer.Init();

    m_scene.Reset();

	m_renderer.p_physX = &m_physx;
	m_renderer.p_models = &m_models;

    m_models["Couch"] = Model("Couch.obj");
    m_models["Cube"] = Model("Cube.obj");
    m_models["Sphere"] = Model("Sphere.obj");
    m_models["HalfSphere"] = Model("HalfSphere.obj");
    m_models["HalfSphere2"] = Model("HalfSphere2.obj");
    m_models["UncappedCylinder"] = Model("UncappedCylinder.obj");
    m_models["SphereLines"] = Model("SphereLines.obj");
    m_models["CubeLines"] = Model("CubeLines.obj");
    m_models["HalfSphereLines"] = Model("HalfSphereLines.obj");
    m_models["HalfSphereLines2"] = Model("HalfSphereLines2.obj");
    m_models["UncappedCylinderLines"] = Model("UncappedCylinderLines.obj");

	CoreGL::SetCamera(&m_camera);

    Texture::LoadTexture("Couch_ALB.png");
    Texture::LoadTexture("CharSheet.png");
    Texture::LoadTexture("Nurse_ALB.png");;
    Texture::LoadTexture("Nurse2_ALB.png");
    Texture::LoadTexture("red.png");

    couchEntity = Entity(&m_models["Couch"], Texture::GetTexIDByTexName("Couch_ALB.png"));
    couchEntity.m_transform.position.z -= 1;
    couchEntity.UpdateCollisionObject();
}

void HellEngine::Update()
{
    static double lastFrame = CoreGL::GetGLTime();
    double deltaTime = CoreGL::GetGLTime() - lastFrame;
    lastFrame = CoreGL::GetGLTime();

    CheckForKeyPresses();

    m_camera.Update(deltaTime);
	
   // if (Input::s_keyPressed[HELL_KEY_P])
        m_physx.StepPhysics();
}

void HellEngine::CheckForKeyPresses()
{
	if (Input::s_keyPressed[HELL_KEY_F]) {
		CoreGL::ToggleFullScreen();
		m_renderer.ReconfigureFrameBuffers();
	}        
	
	if (Input::s_keyPressed[HELL_KEY_H])
		m_renderer.HotLoadShaders();

	if (Input::s_keyPressed[HELL_KEY_SPACE])
		m_scene.NewRagdoll();

	if (Input::s_keyPressed[HELL_KEY_R])
		m_scene.Reset();
}

void HellEngine::Render()
{
    m_camera.CalculateProjectionMatrix(CoreGL::s_windowWidth, CoreGL::s_windowHeight);
    m_camera.CalculateMatrices();
        
	glBindBuffer(GL_UNIFORM_BUFFER, m_renderer.m_uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(m_camera.m_projectionMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, m_renderer.m_uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_camera.m_viewMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

	m_renderer.RenderFrame(m_scene.gameCharacters, couchEntity);
}