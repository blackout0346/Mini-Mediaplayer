#include "raylib.h"
#include <filesystem>
#include <string>
#include <iostream>
#include <vector>
#include "Audio.h"





int main()
{
    setlocale(LC_ALL, "RUS");
    int x = 1280;
    int y = 720;
    InitWindow(x, y, "MusicPlayer");
    InitAudioDevice();
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
    SetTargetFPS(60);
    
    Texture2D playIcon = LoadTexture("Image/Play.png");
    Texture2D prevIcon = LoadTexture("Image/Prev.png");
    Texture2D pauseIcon = LoadTexture("Image/Pause.png");
    Texture2D nextIcon = LoadTexture("Image/Next.png");

    AudioPlayer audioPlayer;
    Rectangle progressBar = { 100, 600, 1080, 20 };

    bool isPlaying = false; // Отслеживаем, играет ли музыка

    float buttonSize = 50;  // Размер кнопок
    float buttonY = 630;    // Координата Y для кнопок

    Rectangle playButton = { 600, buttonY, buttonSize, buttonSize };
    Rectangle prevButton = { 500, buttonY, buttonSize, buttonSize };
    Rectangle nextButton = { 700, buttonY, buttonSize, buttonSize };

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Color { 255, 242, 242 });
    
        audioPlayer.Update();
     
        Vector2 mousePos = GetMousePosition();
      
        // Обработка нажатий мышки
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (CheckCollisionPointRec(mousePos, playButton))
            {
                audioPlayer.PlayPause();
                isPlaying = !isPlaying;  
            }
            if (CheckCollisionPointRec(mousePos, nextButton))
            {
                audioPlayer.Next();
            }
            if (CheckCollisionPointRec(mousePos, prevButton))
            {
                audioPlayer.Prev();
            }
        }


        if (IsKeyPressed(KEY_SPACE))
        {
            audioPlayer.PlayPause();
            isPlaying = !isPlaying;
        }
        if (IsKeyPressed(KEY_RIGHT)) audioPlayer.Next();
        if (IsKeyPressed(KEY_LEFT)) audioPlayer.Prev();
        if (IsKeyPressed(KEY_UP)) audioPlayer.ChangeSpeed(0.1f);
        if (IsKeyPressed(KEY_DOWN)) audioPlayer.ChangeSpeed(-0.1f);

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            Vector2 mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, progressBar))
            {
                float percent = (mousePos.x - progressBar.x) / progressBar.width;
                audioPlayer.Seek(percent);
            }


        }

        float progress = audioPlayer.GetProgress();
        DrawRectangleRec(progressBar, LIGHTGRAY);
        DrawRectangle(progressBar.x, progressBar.y, progressBar.width * progress, progressBar.height, RED);


        float currentTime = audioPlayer.GetCurrentTime();
        float totalTime = audioPlayer.GetTotalTime();
        char currentTimeText[20], totalTimeText[20];
        sprintf_s(currentTimeText, sizeof(currentTimeText), "%02d:%02d", (int)(currentTime) / 60, (int)(currentTime) % 60);
        sprintf_s(totalTimeText, sizeof(totalTimeText), "%02d:%02d", (int)(totalTime) / 60, (int)(totalTime) % 60);
        DrawText(currentTimeText, 20, 600, 20, BLACK);
        DrawText(totalTimeText, 1200, 600, 20, BLACK);
        audioPlayer.DrawMusicList();
        DrawText(audioPlayer.GetCurrentSongName().c_str(), 20, 550, 30, BLACK);
        DrawText("SPACE - Play/Pause | RIGHT - Next | LEFT - Previous", 20, 20, 20, BLACK);
        DrawText("UP - Speed Up | DOWN - Slow Down", 20, 50, 20, BLACK);
        DrawText(TextFormat("Speed: %.1fx", audioPlayer.GetSpeed()), 20, 630, 20, RED);
        DrawTexture(prevIcon, prevButton.x, prevButton.y, WHITE);
        DrawTexture(nextIcon, nextButton.x, nextButton.y, WHITE);
        if (isPlaying)
            DrawTexture(pauseIcon, playButton.x, playButton.y, RED);
        else
            DrawTexture(playIcon, playButton.x, playButton.y, WHITE);
    
        EndDrawing();
    }

    
    UnloadTexture(playIcon);
    UnloadTexture(pauseIcon);
    UnloadTexture(prevIcon);
    UnloadTexture(nextIcon);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
