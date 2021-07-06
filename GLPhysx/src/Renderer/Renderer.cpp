#include "Renderer.h"
#include "Renderer/TextBlitter.h"
#include <random>
#include "Helpers/FileImporter.h"

/*Shader Renderer::s_test_shader;
Shader Renderer::s_solid_color_shader;
Shader Renderer::s_textued_2D_quad_shader;
Shader Renderer::s_skinned_model_shader;
GBuffer Renderer::s_gBuffer;

// Pointers
PhysX* Renderer::p_physX; 
std::unordered_map<std::string, Model>* Renderer::p_models;
*/

void Renderer::Init()
{
    m_test_shader = Shader("test.vert", "test.frag");
    m_solid_color_shader = Shader("solidColor.vert", "solidColor.frag");
    m_textued_2D_quad_shader = Shader("textured2DquadShader.vert", "textured2DquadShader.frag");
    m_skinned_model_shader = Shader("skinnedmodel.vert", "skinnedmodel.frag");

    /*
    m_shaders["Test"] = Shader("test.vert", "test.frag");
    m_shaders["SolidColor"] = Shader("solidColor.vert", "solidColor.frag");
    m_shaders["TexturedQuad"] = Shader("textured2DquadShader.vert", "textured2DquadShader.frag");
    m_shaders["SkinnedModel"] = Shader("skinnedmodel.vert", "skinnedmodel.frag");*/

    s_gBuffer = GBuffer(CoreGL::s_windowWidth, CoreGL::s_windowHeight);


    glUniformBlockBinding(m_skinned_model_shader.ID, glGetUniformBlockIndex(m_skinned_model_shader.ID, "Matrices"), 0);
    glUniformBlockBinding(m_test_shader.ID, glGetUniformBlockIndex(m_test_shader.ID, "Matrices"), 0);

    glGenBuffers(1, &m_uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uboMatrices, 0, 2 * sizeof(glm::mat4));

}


void Renderer::RenderFrame(std::vector<GameCharacter>& gameCharacters, Entity& couch)
{
    DrawGrid();
    

    if (Input::s_showBulletDebug)
        DrawPhysicsWorld();
    else
    {

        // Draw couch
        glEnable(GL_DEPTH_TEST);

        Shader* shader = &m_test_shader;
        shader->use();
        couch.DrawEntity(shader);

        // sphere.DrawEntity(&s_test_shader);

         // Draw ragdolls
        shader = &m_skinned_model_shader;
        shader->use(); 
        shader->setBool("hasAnimation", true);
        shader->setMat4("model", glm::mat4(1));

        for (GameCharacter& gameCharacter : gameCharacters)
            gameCharacter.RenderSkinnedModel(shader);
    }

    TextBlitterPass(&m_textued_2D_quad_shader);
    RenderFinalImage();

    Shader* shader = &m_solid_color_shader;
    shader->use();
    glDisable(GL_DEPTH_TEST);
    glViewport(0, 0, CoreGL::s_windowWidth, CoreGL::s_windowHeight);


    /*
    if (false)//if (Input::s_keyPressed[HELL_KEY_SPACE])
    {
      //createDynamic(camera, , camera.rotate(PxVec3(0, 0, -1)) * 200);
       
        auto mMaterial = p_physX->mPhysics->createMaterial(0.5f, 0.5f, 0.6f);

        PxTransform t = PxTransform();
        t.p.x = camera->m_transform.position.x;
        t.p.y = camera->m_transform.position.y;
       t.p.z = camera->m_transform.position.z;

        PxVec3 velocity;
        velocity.x = camera->m_transform.rotation.x;
        velocity.y = camera->m_transform.rotation.y;
        velocity.z = camera->m_transform.rotation.z;
        velocity.x = camera->m_Front.x;
        velocity.y = camera->m_Front.y;
        velocity.z = camera->m_Front.z;

        velocity *= 200;

        PxRigidDynamic* dynamic = PxCreateDynamic(*p_physX->mPhysics, t, PxSphereGeometry(1.0f), *mMaterial, 10.0f);
        if (dynamic) {
            dynamic->setAngularDamping(0.5f);
            dynamic->setLinearVelocity(velocity);
            p_physX->mScene->addActor(*dynamic);
        }
    }*/
      


    
  

    // Bullet Debug
  //  if (Input::s_showBulletDebug) {
    {
        shader = &m_solid_color_shader;
        shader->use();
        glDisable(GL_DEPTH_TEST);
        glViewport(0, 0, CoreGL::s_windowWidth, CoreGL::s_windowHeight);
      //  DrawPoint(&s_solid_color_shader, glm::vec3(0, 1, 0), glm::vec3(0, 1, 1));

    }
}

void Renderer::DrawGrid()
{
    glBindFramebuffer(GL_FRAMEBUFFER, s_gBuffer.ID);
    glViewport(0, 0, CoreGL::s_windowWidth, CoreGL::s_windowHeight);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Shader* shader = &m_solid_color_shader;
    shader->use();
    shader->setMat4("model", glm::mat4(1));
    int grid_size = 20;
    float grid_scale = 0.25;
    for (int x = -grid_size / 2; x < grid_size / 2 + 1; x++) {
        for (int z = -grid_size / 2; z < grid_size / 2 + 1; z++) {
            Line line;
            line.start_color = glm::vec3(0.5);
            line.end_color = glm::vec3(0.5);
            line.start_pos = glm::vec3(x * grid_scale, 0, -grid_size / 2 * grid_scale);
            line.end_pos = glm::vec3(x * grid_scale, 0, grid_size / 2 * grid_scale);
            DrawLine(shader, line, glm::mat4(1));
            line.start_color = glm::vec3(0.5);
            line.end_color = glm::vec3(0.5);
            line.start_pos = glm::vec3(-grid_size / 2 * grid_scale, 0, z * grid_scale);
            line.end_pos = glm::vec3(grid_size / 2 * grid_scale, 0, z * grid_scale);
            DrawLine(shader, line, glm::mat4(1));
        }
    }
}

void Renderer::TextBlitterPass(Shader* shader)
{
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture::GetTexIDByTexName("CharSheet.png"));
    TextBlitter::BlitText(TextBlitter::s_textToBlit, false); // this congfigures internal blitter variables
    TextBlitter::DrawTextBlit(shader);                       // this draws the thing
    TextBlitter::ResetBlitter();                             // resets internal variables
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::DrawLine(Shader* shader, Line line, glm::mat4 modelMatrix = glm::mat4(1))
{
    static unsigned int VAO = 0;
    static unsigned int VBO;
    if (VAO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }
    float vertices[] = {
        line.start_pos.r,  line.start_pos.g,  line.start_pos.b,  line.start_color.r,  line.start_color.g,  line.start_color.b,
        line.end_pos.r,  line.end_pos.g,  line.end_pos.b,  line.end_color.r,  line.end_color.g,  line.end_color.b,
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    shader->setMat4("model", modelMatrix);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 6);
}

void Renderer::HotLoadShaders()
{
    m_test_shader.ReloadShader();
    m_solid_color_shader.ReloadShader();
    m_textued_2D_quad_shader.ReloadShader();
    m_skinned_model_shader.ReloadShader();
}

void Renderer::DrawPoint(Shader* shader, glm::vec3 position, glm::vec3 color)
{
    static unsigned int VAO = 0;
    static unsigned int VBO;
    if (VAO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }

    float vertices[] = {
        position.x, position.y, position.z, color.r, color.g, color.b,
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glm::mat4 modelMatrix = glm::mat4(1);
    shader->setMat4("model", modelMatrix);
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 1);
}

void Renderer::RenderFinalImage()
{
    Shader* shader = &m_textued_2D_quad_shader;
    shader->use();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s_gBuffer.gAlbedo);
    glViewport(0, 0, CoreGL::s_windowWidth, CoreGL::s_windowHeight);

    static GLuint VAO = 0;
    if (VAO == 0)
    {
        float quadVertices[] = {
            // positions         texcoords
            -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
                1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
                1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        };
        unsigned int VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }

    shader->setMat4("model", glm::mat4(1));
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}


void Renderer::DrawPhysicsWorld()
{
    bool drawSolids = true;

    int primitiveType = GL_TRIANGLES;


    Shader* shader = &m_solid_color_shader;
    shader->use();

    PxScene* scene;
    PxGetPhysics().getScenes(&scene, 1);
    PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
    if (nbActors)
    {
        std::vector<PxRigidActor*> actors(nbActors);
        scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);

        PxShape* shapes[128];
        int numActors = static_cast<PxU32>(actors.size());

        for (PxU32 i = 0; i < numActors; i++)
        {
            const PxU32 nbShapes = actors[i]->getNbShapes();

            PX_ASSERT(nbShapes <= MAX_NUM_ACTOR_SHAPES);
            actors[i]->getShapes(shapes, nbShapes);
            const bool sleeping = actors[i]->is<PxRigidDynamic>() ? actors[i]->is<PxRigidDynamic>()->isSleeping() : false;

            for (PxU32 j = 0; j < nbShapes; j++)
            {
                const PxMat44 shapePose(PxShapeExt::getGlobalPose(*shapes[j], *actors[i]));
                const PxGeometryHolder h = shapes[j]->getGeometry();
                const PxGeometry geom = h.any();

                auto shape = shapes[j]->getGeometry();

                auto PxMatrix = shapePose;

                glm::mat4 matrix;
                for (int x = 0; x < 4; x++)
                    for (int y = 0; y < 4; y++)
                        matrix[x][y] = PxMatrix[x][y];

                auto models = *p_models;

                if (geom.getType() == PxGeometryType::eBOX) {

                    Transform transform;
                    transform.scale = glm::vec3(shape.box().halfExtents.x * 2, shape.box().halfExtents.y * 2, shape.box().halfExtents.z * 2);

                    glDepthFunc(GL_LEQUAL);
                    shader->setVec3("color", glm::vec3(1, 1, 1));
                    models["CubeLines"].Draw(shader, matrix * transform.to_mat4(), GL_LINES);

                    if (drawSolids) {
                        shader->setVec3("color", glm::vec3(0.4, 0.4, 0.4));
                        models["Cube"].Draw(shader, matrix * transform.to_mat4(), GL_TRIANGLES);
                    }


                    shader->setVec3("color", glm::vec3(0.4, 0.4, 0.4));
                }
                if (shape.any().getType() == PxGeometryType::eSPHERE)
                {
                    Transform transform;
                    transform.scale = glm::vec3(shape.sphere().radius);

                    if (drawSolids) {
                        shader->setVec3("color", glm::vec3(0.4, 0.4, 0.4));
                        models["Sphere"].Draw(shader, matrix * transform.to_mat4(), GL_TRIANGLES);
                    }

                    shader->setVec3("color", glm::vec3(1, 1, 1));
                    models["SphereLines"].Draw(shader, matrix * transform.to_mat4(), GL_LINES);
                }

                if (geom.getType() == PxGeometryType::eCAPSULE)
                {
                    float halfExtent = shape.capsule().halfHeight;
                    float radius = shape.capsule().radius;

                    Transform cylinderTransform;
                    cylinderTransform.scale.x = halfExtent * 2;
                    cylinderTransform.scale.y = radius;
                    cylinderTransform.scale.z = radius;
                    Transform topSphereTransform;
                    topSphereTransform.position.x += halfExtent;
                    topSphereTransform.scale = glm::vec3(radius);
                    Transform bottomSphereTransform;
                    bottomSphereTransform.position.x -= halfExtent;
                    bottomSphereTransform.scale = glm::vec3(radius);

                    if (drawSolids) {
                        shader->setVec3("color", glm::vec3(0.4, 0.4, 0.4));
                        models["UncappedCylinder"].Draw(shader, matrix * cylinderTransform.to_mat4(), GL_TRIANGLES);
                        models["HalfSphere2"].Draw(shader, matrix * bottomSphereTransform.to_mat4(), GL_TRIANGLES);
                        models["HalfSphere"].Draw(shader, matrix * topSphereTransform.to_mat4(), GL_TRIANGLES);
                    }

                    shader->setVec3("color", glm::vec3(1, 1, 1));
                    models["UncappedCylinderLines"].Draw(shader, matrix * cylinderTransform.to_mat4(), GL_LINES);
                    models["HalfSphereLines"].Draw(shader, matrix * topSphereTransform.to_mat4(), GL_LINES);
                    models["HalfSphereLines2"].Draw(shader, matrix * bottomSphereTransform.to_mat4(), GL_LINES);


                }

            }
        }
    }
}

void Renderer::ReconfigureFrameBuffers()
{
    s_gBuffer.Configure(CoreGL::s_windowWidth, CoreGL::s_windowHeight);
}

