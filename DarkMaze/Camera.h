#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "src/labyrinth/labyrinth.h"
#include "Item.h"
#include <GLFW/glfw3.h>
#include <iostream>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    //int keysCollected = 0;
    float baseLightRange = 2.5f;      // ZMNIEJSZONE: Startowa widoczność (było 4.0)
    float boostedLightRange = 10.0f;  // Zasięg po baterii
    float lightEffectEndTime = 0.0f;  // Czas wygaśnięcia bonusu
    bool isGameOver = false; // Dodaj to w sekcji public:
    int (*maze)[Labyrinth::COLS];

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime, std::vector<Item>& activeItems)
    {
        float velocity = MovementSpeed * deltaTime;
        moveWithSliding(directionVector(direction), velocity, activeItems);
    }

    glm::vec3 directionVector(Camera_Movement direction) {
        if (direction == FORWARD) return Front;
        if (direction == BACKWARD) return -Front;
        if (direction == LEFT) return -Right;
        if (direction == RIGHT) return Right;
        return glm::vec3(0.0f);
        /*if (direction == FORWARD)
            moveWithSliding(Front, velocity, activeItems);

        if (direction == BACKWARD)
            moveWithSliding(-Front, velocity, activeItems);
        if (direction == LEFT)
            moveWithSliding(-Right, velocity, activeItems);
        if (direction == RIGHT)
            moveWithSliding(Right, velocity, activeItems); */
        
    }

    void moveWithSliding(glm::vec3 direction, float velocity, std::vector<Item>& activeItems)

    {
        glm::vec3 desired = direction * velocity;
        glm::vec3 newPos = Position;

        // --- X movement ---
        glm::vec3 tryX = newPos;
        tryX.x += desired.x;
        if (!wallsCollision(tryX, activeItems)) {
            newPos.x += desired.x;
        }

        // --- Z movement ---
        glm::vec3 tryZ = newPos;
        tryZ.z += desired.z;
        if (!wallsCollision(tryZ, activeItems)) {
            newPos.z += desired.z;
        }

        Position = newPos;
    }
    

    bool cubeCollision(glm::vec3 cubeMax, glm::vec3 cubeMin, glm::vec3 pos) {
        return (pos.x >= cubeMin.x && pos.x <= cubeMax.x) &&
               (pos.y >= cubeMin.y && pos.y <= cubeMax.y) &&
               (pos.z >= cubeMin.z && pos.z <= cubeMax.z);
    }


    bool wallsCollision(glm::vec3 pos, const std::vector<Item>& activeItems) {

        float cellSize = 1.0f;

        // 1) Kolizja ze ścianami labiryntu
        for (int i = 0; i < Labyrinth::ROWS; i++) {
            for (int j = 0; j < Labyrinth::COLS; j++) {
                if (maze[i][j] == 1) {

                    glm::vec3 cubeMax(
                        j * cellSize + 0.6f,
                        1.1f,
                        i * cellSize + 0.6f
                    );

                    glm::vec3 cubeMin(
                        j * cellSize - 0.6f,
                        -0.1f,
                        i * cellSize - 0.6f
                    );

                    if (cubeCollision(cubeMax, cubeMin, pos))
                        return true;
                }
            }
        }

        // 2) Kolizja z DRZWIAMI (EXIT)
        for (const auto& item : activeItems) {

            if (item.type != ItemType::EXIT)
                continue;

            // drzwi ZAMKNIĘTE → blokują jak ściana
            glm::vec3 doorMax = item.position + glm::vec3(0.4f, 1.0f, 0.4f);
            glm::vec3 doorMin = item.position - glm::vec3(0.4f, 0.0f, 0.4f);

            if (cubeCollision(doorMax, doorMin, pos))
                return true;
        }

        return false;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif