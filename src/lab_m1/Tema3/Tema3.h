#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "components/simple_scene.h"
#include "components/transform.h"
#include "components/camera.h"

#include "header/player.h"
#include "header/fir.h"
#include "header/gift.h"
#include "header/rock.h"
#include "header/lantern.h"
#include "header/plane.h"


namespace tema3
{
    class Tema3 : public gfxc::SimpleScene
    {
     public:
        Tema3();
        ~Tema3();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D *texture, glm::vec2 offsetUV = glm::vec2(0, 0));
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color);
        void setLightData(Mesh* mesh, Shader* shader);
        void setBasicData(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);

        Texture2D *CreateRandomTexture(unsigned int width, unsigned int height);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        std::unordered_map<std::string, Texture2D *> mapTextures;

        template<typename Base, typename T>
        inline bool instanceof(const T* ptr) {
            return dynamic_cast<const Base*>(ptr) != nullptr;
        }

        inline void PopulateSceneLightsVector();

        inline void PopulateObjVector();
        inline Object* CreateObject(glm::vec3 pos);
        inline void FreeObjVector();
        bool ValidatePos(glm::vec3 pos);
        glm::vec3 GenerateSpawnPosition();

        void RenderObjects();
        inline void RenderPlane(Plane* plane);
        inline void RenderPlayer(Player* player);
        inline void RenderFir(Fir* fir);
        inline void RenderRock(Rock* rock);
        inline void RenderLantern(Lantern* lantern);
        inline void RenderGift(Gift* gift);

        // Plane variables
        Plane game_plane;
        float plane_angle;
        glm::mat4 rotateWorld;
        glm::vec3 plane_dir;
        glm::vec3 plane_up;
        glm::vec2 UV_offset;
        glm::vec2 UV_offset2;

        // Player variable
        Player player;
        glm::vec3 player_pos;
        glm::vec3 player_dir;
        glm::vec4 player_forward;

        // Environment variable
        std::vector<Object*> env;
        int object_num;
        float MAX_dist_Z, MIN_dist_Z;
        float MAX_dist_X;
        float Remove_dist;

        // Camera
        gfxc::Camera* camera;
        glm::vec3 camera_offset;

        // Game variables
        bool paused;
        int score;

        // Light Variables
        std::vector<light_source> scene_lights;
        float light_number;
        glm::vec3 colorAmbientalGlobal;
        glm::vec3 temp_color_ke;
        float temp_material_ka;
        light_source dir_light;
    };
}   // namespace tema3
