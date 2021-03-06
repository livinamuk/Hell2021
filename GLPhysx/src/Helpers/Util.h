#pragma once
#include "Header.h"
#include "Renderer/Transform.h"
#include "Renderer/Shader.h"

#include "rapidjson/document.h"
#include <rapidjson/filereadstream.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include "assimp/matrix4x4.h"
#include "assimp/matrix3x3.h"

#include "Physics/PhysX.h"

class Util
{
public:
	static PxQuat PxQuatFromJSONArray(rapidjson::GenericArray<false, rapidjson::Value> const arr);
	static PxVec3 PxVec3FromJSONArray(rapidjson::GenericArray<false, rapidjson::Value> const arr);
	static PxMat44 GlmMat4ToPxMat44(glm::mat4 matrix);
	static PxMat44 PxMat4FromJSONArray(rapidjson::GenericArray<false, rapidjson::Value> const arr);

	static glm::mat4 PxMat44ToGlmMat4(PxMat44 matrix);
	static bool GetRayPlaneIntersecion(const glm::vec3& origin, const glm::vec3& dir, const glm::vec3& plane_point, const glm::vec3& normal, float& out);
	static glm::vec3 GetMouseRay(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, int mouseX, int mouseY, int screenWidth, int screenHeight);
	static void PrintMat4(glm::mat4 m);
	static void PrintVec3(glm::vec3 v);
	static std::string FileNameFromPath(std::string filepath);
	static FileType FileTypeFromPath(std::string filepath);
	static void SetNormalsAndTangentsFromVertices(Vertex* vert0, Vertex* vert1, Vertex* vert2);
	static glm::vec3 NormalFromTriangle(glm::vec3 pos0, glm::vec3 pos1, glm::vec3 pos2);

	
	static std::string PhysicsObjectEnumToString(PhysicsObjectType objectType); 
	
	static std::string BoolToString(bool boolean);

	static glm::mat4 Mat4FromJSONArray(rapidjson::GenericArray<false, rapidjson::Value> const arr);
	static glm::vec3 Vec3FromJSONArray(rapidjson::GenericArray<false, rapidjson::Value> const arr);

	static glm::vec3 GetTranslationFromMatrix(glm::mat4 matrix);

	static void InterpolateQuaternion(glm::quat& pOut, const glm::quat& Start, const glm::quat& End, float Factor);		
	static glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from);
	static glm::mat4 aiMatrix3x3ToGlm(const aiMatrix3x3& from);
	static glm::mat4 Mat4InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
	static glm::mat4 Mat4InitRotateTransform(float RotateX, float RotateY, float RotateZ);
	static glm::mat4 Mat4InitTranslationTransform(float x, float y, float z);		
	static bool StrCmp(const char* queryA, const char* queryB); 
	static const char* CopyConstChar(const char* text);
};
