#include "lab_m1/Tema3/Tema3.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace tema3;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}


void Tema3::Init()
{
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "textures");

    camera = GetSceneCamera();

    paused = false;
    score = 0;

    srand(time(NULL));

    // Init variables
    plane_angle = glm::radians(30.f);
    object_num = 25;

    rotateWorld = glm::rotate(glm::mat4(1.0f), plane_angle, glm::vec3_right);
    {
        glm::vec4 forward = rotateWorld * glm::vec4(0.f, 0.f, 1.0f, 1.0f);
        plane_dir = glm::normalize(glm::vec3(forward));
        plane_up = glm::cross(glm::vec3_left, plane_dir);
    }

    player_pos = glm::vec3(0, 0.7f, 0);
    player_dir = glm::vec3(0, 0, 1);
    game_plane = Plane(player_pos, plane_angle);
    UV_offset = glm::vec2(0, 0);
    UV_offset2 = glm::vec2(0, 0);

    player = Player(player_pos, plane_angle);
    player.setQuatRotation(plane_dir, plane_up);
    player.setLocalUp(plane_up);

    camera_offset = glm::vec3(0.f, 1.1f, 25.f);
    camera->SetPosition(player_pos + camera_offset);
    camera->RotateOX(RADIANS(-80));

    // Insert game objects in vector
    MAX_dist_Z = 110;
    MIN_dist_Z = 30;
    MAX_dist_X = 25;
    Remove_dist = 28.f;

    PopulateObjVector();

    // Light set-up
    light_number = 0;
    colorAmbientalGlobal = glm::vec3(0.25f, 0.25f, 0.25f);
    dir_light = light_source(DIRECTIONAL, glm::vec3(0), glm::vec3(0), glm::vec3(0.5f, 0.5f, 1.f), 0, 0.1f, 0.5f, 0.9f);

    // Load textures
        
        // --- Fir ---
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "fir.jpg").c_str(), GL_CLAMP_TO_EDGE);
        mapTextures["fir_body"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "pine.jpg").c_str(), GL_REPEAT);
        mapTextures["fir_trunk"] = texture;
    }

        // --- Gift ---
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "gift.jpg").c_str(), GL_MIRRORED_REPEAT);
        mapTextures["gift"] = texture;
    }

        // --- Ground ---
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "snow.jpg").c_str(), GL_REPEAT);
        mapTextures["ground"] = texture;
    }

        // --- Rock ---
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "rock.png").c_str(), GL_REPEAT);
        mapTextures["rock"] = texture;
    }

        // --- Lantern ---
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "crate.jpg").c_str(), GL_REPEAT);
        mapTextures["lantern"] = texture;
    }

    // Load meshes
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cone");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("WindShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader_windy.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}

void Tema3::PopulateObjVector() {
    int i = env.size();
    for (i; i < object_num; i++) {
        Object* obj;

        glm::vec3 pos;
        do {
            pos = GenerateSpawnPosition();
        } while (ValidatePos(pos) == false);

        obj = CreateObject(pos);

        env.push_back(obj);
    }
}

Object* Tema3::CreateObject(glm::vec3 pos) {
    int type = rand() % 4;

    switch (type) {
    case 0: return new Fir(pos, plane_angle); break;
    case 1: return new Gift(pos, plane_angle); break;
    case 2: return new Rock(pos, plane_angle); break;
    default: return new Lantern(pos, plane_up, plane_angle);
    }
}

bool Tema3::ValidatePos(glm::vec3 currentPos) {
    if (env.size() == 0) return true;

    for (Object* obj : env) {

        glm::vec3 pos = obj->getPos();

        if (glm::distance(currentPos, pos) < 10.f)
            return false;
    }

    return true;
}

glm::vec3 Tema3::GenerateSpawnPosition() {
    int rand_dist_Z = rand() % (int)(MAX_dist_Z - MIN_dist_Z + 1);
    int rand_dist_X = rand() % (int)MAX_dist_X;
    int left = (rand() % 2 == 0) ? 1 : -1;

    glm::vec3 pos = player.getPos() +  plane_dir * (MIN_dist_Z + rand_dist_Z) + glm::vec3_right * (float)(rand_dist_X * left);

    return pos;
}


void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema3::Update(float deltaTimeSeconds)
{
    if (paused)
        return;

    player.update(player_dir, deltaTimeSeconds);
    game_plane.update(player.getPos(), deltaTimeSeconds);
    camera->SetPosition(player.getPos() + camera_offset);

    UV_offset += UV_offset2 * deltaTimeSeconds;

    RenderObjects();
}

void Tema3::RenderObjects() {
    RenderPlane(&game_plane);
    RenderPlayer(&player);
    
    for (Object* obj : env) {

        temp_material_ka = obj->getMaterialKa();
        temp_color_ke = obj->getColorKe();

        if (instanceof<Fir>(obj)) {
            RenderFir((Fir*)obj);
        }
        else if (instanceof<Gift>(obj)) {
            RenderGift((Gift*)obj);
        }
        else if (instanceof<Rock>(obj)) {
            RenderRock((Rock*)obj);
        }
        else if (instanceof<Lantern>(obj)) {
            RenderLantern((Lantern*)obj);
        }
    }
}

void Tema3::RenderPlane(Plane* plane) {
    temp_material_ka = plane->getMaterialKa();
    temp_color_ke = plane->getColorKe();

    RenderSimpleMesh(meshes["plane"], shaders["LabShader"], plane->getMatrixComp()["plane"], mapTextures["ground"], UV_offset);
}

void Tema3::RenderPlayer(Player* player) {
    temp_material_ka = player->getMaterialKa();
    temp_color_ke = player->getColorKe();

    RenderSimpleMesh(meshes["box"], shaders["LabShader"], player->getMatrixComp()["player_head"], glm::vec3(1, 0, 0));
    RenderSimpleMesh(meshes["box"], shaders["LabShader"], player->getMatrixComp()["player_ski_left"], glm::vec3(0, 0, 1));
    RenderSimpleMesh(meshes["box"], shaders["LabShader"], player->getMatrixComp()["player_ski_right"], glm::vec3(0, 0, 1));
}

void Tema3::RenderFir(Fir* fir) {
    RenderSimpleMesh(meshes["box"], shaders["LabShader"], fir->getMatrixComp()["fir_trunk"], mapTextures["fir_trunk"]);
    RenderSimpleMesh(meshes["cone"], shaders["WindShader"], fir->getMatrixComp()["fir_body_middle"], mapTextures["fir_body"]);
    RenderSimpleMesh(meshes["cone"], shaders["WindShader"], fir->getMatrixComp()["fir_body_upper"], mapTextures["fir_body"]);
}

void Tema3::RenderRock(Rock* rock) {
    RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], rock->getMatrixComp()["rock_1"], mapTextures["rock"]);
    RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], rock->getMatrixComp()["rock_2"], mapTextures["rock"]);
    RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], rock->getMatrixComp()["rock_3"], mapTextures["rock"]);
    RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], rock->getMatrixComp()["rock_4"], mapTextures["rock"]);
}

void Tema3::RenderLantern(Lantern* lantern) {
    RenderSimpleMesh(meshes["box"], shaders["LabShader"], lantern->getMatrixComp()["pillar_1"], mapTextures["lantern"]);
    RenderSimpleMesh(meshes["box"], shaders["LabShader"], lantern->getMatrixComp()["pillar_2"], mapTextures["lantern"]);
}

void Tema3::RenderGift(Gift* gift) {
    RenderSimpleMesh(meshes["box"], shaders["LabShader"], gift->getMatrixComp()["gift"], mapTextures["gift"]);
}

void Tema3::FrameEnd()
{
    if (paused)
        return;

    glm::vec3 player_pos = player.getPos();

    for (int i = 0; i < env.size(); i++) {
        Object* obj = env[i];
        
        glm::vec3 obj_pos = obj->getPos();

        if (player_pos.z - obj_pos.z > Remove_dist) {
            env.erase(env.begin() + i);
            delete obj;
            continue;
        }

        if (player.ColisionAABB(obj)) {
            if (instanceof<Gift>(obj)) {
                std::cout << "Gift collected!\n";
                score += 1;
            }
            else {
                FreeObjVector();
                paused = true;

                std::cout << "Game Over! Press \"R\" to restart!\n";
                std::cout << "Your score is " << score << "\n";

                return;
            }

            env.erase(env.begin() + i);
            delete obj;
            continue;
        }
    }

    PopulateObjVector();
}

void Tema3::FreeObjVector() {
    for (int i = 0; i < env.size(); i++) {
        Object* obj = env[i];
        delete obj;
    }

    env.clear();
}

void Tema3::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture, glm::vec2 offsetUV)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set basic data for shader
    setBasicData(mesh, shader, modelMatrix);

    // Set shader uniform property for texture
    GLint location5 = glGetUniformLocation(shader->program, "has_texture");
    glUniform1i(location5, 1);

    GLint location6 = glGetUniformLocation(shader->program, "u_offsetUV");
    glUniform2fv(location6, 1, glm::value_ptr(offsetUV));

    if (texture)
    {
        // - activate texture location 0
        glActiveTexture(GL_TEXTURE0);

        // - bind the texture1 ID
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());

        // - send theuniform value
        glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set basic data for shader
    setBasicData(mesh, shader, modelMatrix);

    // Set shader uniform property for texture
    GLint location1 = glGetUniformLocation(shader->program, "has_texture");
    glUniform1i(location1, 0);

    // Set shader solid color
    GLint location2 = glGetUniformLocation(shader->program, "u_color");
    glUniform3fv(location2, 1, glm::value_ptr(color));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema3::setBasicData(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) {
    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Set shader uniform "Time" to Engine::GetElapsedTime()
    GLint location4 = glGetUniformLocation(shader->program, "Time");
    float time = Engine::GetElapsedTime();
    glUniform1f(location4, (GLfloat)(time));

    // Load light scene data to shader
    setLightData(mesh, shader);
}

void Tema3::setLightData(Mesh* mesh, Shader* shader) {
    PopulateSceneLightsVector();

    // Bind lights number
    int loc_light_number = glGetUniformLocation(shader->program, "light_number");
    glUniform1i(loc_light_number, light_number);

    // Bind global ambiental color
    int loc_CAG = glGetUniformLocation(shader->program, "colorAmbientalGlobal");
    glUniform3fv(loc_CAG, 1, glm::value_ptr(colorAmbientalGlobal));

    // Bind eye position
    int loc_eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(loc_eye_position, 1, glm::value_ptr(camera->m_transform->GetWorldPosition()));

    // Ka material
    GLuint loc_ka = glGetUniformLocation(shader->program, "material_ka");
    glUniform1f(loc_ka, temp_material_ka);

    // Light color
    GLuint loc_color_ke = glGetUniformLocation(shader->program, "color_ke");
    glUniform3fv(loc_color_ke, 1, glm::value_ptr(temp_color_ke));

    // Send lights to shader
    for (int i = 0; i < light_number; i++) {
        std::string name;

        // Light type
        name = std::string("lights[") + std::to_string(i) + std::string("].type");
        GLuint loc_type = glGetUniformLocation(shader->program, name.c_str());
        glUniform1i(loc_type, scene_lights[i].type);

        // Light position
        name = std::string("lights[") + std::to_string(i) + std::string("].light_position");
        GLuint loc_position = glGetUniformLocation(shader->program, name.c_str());
        glUniform3fv(loc_position, 1, glm::value_ptr(scene_lights[i].light_position));

        // Light direction
        name = std::string("lights[") + std::to_string(i) + std::string("].light_direction");
        GLuint loc_direction = glGetUniformLocation(shader->program, name.c_str());
        glUniform3fv(loc_direction, 1, glm::value_ptr(scene_lights[i].light_direction));

        // Light color
        name = std::string("lights[") + std::to_string(i) + std::string("].light_color");
        GLuint loc_color = glGetUniformLocation(shader->program, name.c_str());
        glUniform3fv(loc_color, 1, glm::value_ptr(scene_lights[i].light_color));

        // Cut off angle
        name = std::string("lights[") + std::to_string(i) + std::string("].cut_off");
        GLuint loc_cut_off = glGetUniformLocation(shader->program, name.c_str());
        glUniform1f(loc_cut_off, scene_lights[i].cut_off);

        // Kd material
        name = std::string("lights[") + std::to_string(i) + std::string("].material_kd");
        GLuint loc_kd = glGetUniformLocation(shader->program, name.c_str());
        glUniform1f(loc_kd, scene_lights[i].material_kd);

        // Ks material
        name = std::string("lights[") + std::to_string(i) + std::string("].material_ks");
        GLuint loc_ks = glGetUniformLocation(shader->program, name.c_str());
        glUniform1f(loc_ks, scene_lights[i].material_ks);

        // Shininess material
        name = std::string("lights[") + std::to_string(i) + std::string("].material_shininess");
        GLuint loc_n = glGetUniformLocation(shader->program, name.c_str());
        glUniform1f(loc_n, scene_lights[i].material_shininess);
    }

    scene_lights.clear();
}

void Tema3::PopulateSceneLightsVector() {
    scene_lights.push_back(dir_light);
    scene_lights.push_back(player.getLight());

    glm::vec3 player_pos = player.getPos();

    for (Object* obj : env) {

        glm::vec3 obj_pos = obj->getPos();

        if (obj_pos.z - player_pos.z > 18.f)
            continue;

        if (player_pos.z - obj_pos.z > Remove_dist)
            continue;

        if (instanceof<Gift>(obj) || instanceof<Fir>(obj))
            scene_lights.push_back(obj->getLight());
        else if (instanceof<Lantern>(obj)) {
            scene_lights.push_back(((Lantern*)obj)->getSpotlight1());
            scene_lights.push_back(((Lantern*)obj)->getSpotlight2());
        }
    }

    light_number = scene_lights.size();
}


Texture2D* Tema3::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    // TODO(student): Generate random texture data
    for (int i = 0; i < size; i++) {
        data[i] = rand() % 255;
    }

    // TODO(student): Generate and bind the new texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // TODO(student): Set the texture parameters (MIN_FILTER, MAG_FILTER and WRAPPING MODE) using glTexParameteri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    // Use glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // TODO(student): Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
    }
}


void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (paused == true && key == GLFW_KEY_R) {
            paused = false;
            score = 0;

            player.setPos(player_pos);
            PopulateObjVector();
    }
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Change move player direction based on mouse position
    glm::vec2 Res = window->GetResolution();
    float angle;

    if (mouseY < Res.y / 2 || mouseX == Res.x / 2) {
        player_dir = glm::vec3(0.f, 0.f, 1.f);
        angle = M_PI_2;
        UV_offset2 = glm::vec2(cos(angle), sin(angle)) / 100.f * player.getPlayerSpeed();
    }
    else {
        angle = atan((mouseY - Res.y / 2) / (mouseX - Res.x / 2));

        // Limit player angle to (-PI / 4, PI / 4)
        if (0 < angle && angle < M_PI_4) {
            angle = M_PI_4;
        }
        if (angle > -M_PI_4  && angle < 0) {
            angle = -M_PI_4;
        }

        // PI / 2 >= angle > PI / 4
        if (mouseX > Res.x / 2) {
            player_dir = glm::vec3(cos(angle), 0, sin(angle));
            UV_offset2 = glm::vec2(cos(angle), sin(angle)) / 100.f * player.getPlayerSpeed();
        }

        // -PI / 4 > angle >= -PI / 2
        else {
            player_dir = glm::vec3(-cos(angle), 0, -sin(angle));
            UV_offset2 = glm::vec2(cos(M_PI + angle), sin(M_PI + angle)) / 100.f * player.getPlayerSpeed();
        }
    }
    player_forward = rotateWorld * glm::vec4(player_dir, 1.0f);
    player_dir = glm::normalize(glm::vec3(player_forward));
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
