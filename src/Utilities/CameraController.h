#include <raylib.h>
#include <raymath.h>

#pragma once

class CameraController
{
public:
    Camera3D currentCamera;
    float camera_speed;
    float camera_sensitivity;
    float angleX;
    float angleY;

    CameraController()
        : camera_speed(6.0f), camera_sensitivity(0.25f), angleX(0), angleY(0)
    {
        currentCamera = {0};
        currentCamera.position = {2.0f, 1.0f, 0.0f};
        currentCamera.up = {0.0f, 1.0f, 0.0f};
        currentCamera.fovy = 60.0f;
        currentCamera.projection = CAMERA_PERSPECTIVE;
    }

    void Update()
    {
        float deltaTime = GetFrameTime();
        Vector3 direction = Vector3Normalize({(float)(IsKeyDown(KEY_A) - IsKeyDown(KEY_D)),
                                              (float)(IsKeyDown(KEY_E) - IsKeyDown(KEY_Q)),
                                              (float)(IsKeyDown(KEY_W) - IsKeyDown(KEY_S))});

#ifdef PLATFORM_DESKTOP
        Vector2 mouseDelta = GetMouseDelta();
#else
        Vector2 mouseDelta = {0, 0};
        
        if (IsGestureDetected(GESTURE_DRAG)) mouseDelta = Vector2Scale(GetGestureDragVector(), 10);
#endif
        angleX -= camera_sensitivity * mouseDelta.x * deltaTime;

        float newAngleY = angleY - (camera_sensitivity * mouseDelta.y * deltaTime);
        if (abs(newAngleY) < 3.0f)
        {
            angleY = newAngleY;
        }

        float targetX = sin(angleX) + (cos(angleY) * sin(angleX));
        float targetZ = cos(angleX) + (cos(angleY) * cos(angleX));
        float targetY = sin(angleY);

        Vector3 right = Vector3Normalize(Vector3CrossProduct(currentCamera.up, {targetX, targetY, targetZ}));

        Vector3 frontMov = Vector3Scale({targetX, targetY, targetZ}, direction.z);
        Vector3 rightMov = Vector3Scale(right, direction.x);
        Vector3 upMov = Vector3Scale(currentCamera.up, direction.y);

        currentCamera.position = Vector3Add(
            currentCamera.position,
            Vector3Scale(
                Vector3Add(upMov, Vector3Add(frontMov, rightMov)),
                camera_speed * deltaTime));

        currentCamera.target = Vector3Add(currentCamera.position, {targetX, targetY, targetZ});
    }

private:
};