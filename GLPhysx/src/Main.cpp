#include "Header.h"
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

#include "HellEngine.h"

int main()
{
    Input::s_showCursor = false;

    HellEngine hellEngine;
    hellEngine.Init();

    ///////////////////////
    //   Main game loop  //

    while (CoreGL::IsRunning() && !Input::s_keyDown[HELL_KEY_ESCAPE])
    {
        hellEngine.Update();

        // Update OpenGL and get keyboard state
        CoreGL::OnUpdate();
        CoreGL::ProcessInput();
        Input::HandleKeypresses();


        TextBlitter::BlitLine("Ragdoll GL Demo");
        TextBlitter::BlitLine("Space: Spawn ragdoll");
        TextBlitter::BlitLine("R: Reset scene");
        TextBlitter::BlitLine("B: toggle debug");
        TextBlitter::BlitLine(" ");

        // RENDER FRAME
        hellEngine.Render();

        CoreGL::SwapBuffersAndPollEvents();
    }

    CoreGL::Terminate();
    return 0;
}