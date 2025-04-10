#pragma once
#include "raylib.h"
#include <filesystem>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include "Audio.h"


#define MUSIC_FOLDER "All/"
class AudioPlayer
{
private:
    std::vector<std::string> musicNames;
    std::vector<Music> musicArray;
    int currentMusic;
    bool isPaused;
    float speed;
    float scrolling = 0;

public:
    AudioPlayer()
    {
   
        currentMusic = 0;
        isPaused = false;
        speed = 1.0f;
        LoadMusicFromFolder();
       
        
    }

    ~AudioPlayer()
    {
        for (Music& music : musicArray)
        {
            UnloadMusicStream(music);
        }
    }
    void ScrollUp() { scrolling--; }
    void ScrollDown() { scrolling++; }

 
    void LoadMusicFromFolder()
    {
        for (const auto& entry : std::filesystem::directory_iterator(MUSIC_FOLDER))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".mp3")
            {
                musicArray.push_back(LoadMusicStream(entry.path().string().c_str()));

             
                std::string name = entry.path().stem().string();
                musicNames.push_back(name);
            }
        }

        if (musicArray.empty())
        {
            std::cout << "Нет файлов .mp3 в папке " << MUSIC_FOLDER << std::endl;
        }

    }
    std::string GetCurrentSongName()
    {
        if (!musicNames.empty())
        {
            return musicNames[currentMusic]; 
        }
        return "Нет музыки";
    }
    std::vector<std::string> GetSongList()
    {
        return musicNames;
    }
    int GetCurrentSongIndex()
    {
        return currentMusic;
    }

    void PlayPause()
    {
        if (!musicArray.empty())
        {
            if (isPaused)
            {
                ResumeMusicStream(musicArray[currentMusic]);
                isPaused = false;
            }
            else if (IsMusicStreamPlaying(musicArray[currentMusic]))
            {
                PauseMusicStream(musicArray[currentMusic]);
                isPaused = true;
            }
            else
            {
                PlayMusicStream(musicArray[currentMusic]);
                isPaused = false;
                SetMusicPitch(musicArray[currentMusic], speed);
            }
        }
    }

    void Next()
    {
        if (!musicArray.empty())
        {
            StopMusicStream(musicArray[currentMusic]);
            currentMusic = (currentMusic + 1) % musicArray.size();
            PlayMusicStream(musicArray[currentMusic]);
            isPaused = false;
            SetMusicPitch(musicArray[currentMusic], speed);
        }
    }

    void Prev()
    {
        if (!musicArray.empty())
        {
            StopMusicStream(musicArray[currentMusic]);
            currentMusic = (currentMusic - 1 + musicArray.size()) % musicArray.size();
            PlayMusicStream(musicArray[currentMusic]);
            isPaused = false;
            SetMusicPitch(musicArray[currentMusic], speed);
        }
    }

    void ChangeSpeed(float delta)
    {
        speed += delta;
        if (speed < 0.1f) speed = 0.1f;
        if (speed > 4.0f) speed = 4.0f;

        if (!musicArray.empty())
        {
            SetMusicPitch(musicArray[currentMusic], speed);
        }
    }
    void Seek(float percent)
    {
        if (!musicArray.empty() && percent >= 0.0f && percent <= 1.0f)
        {
            float length = GetMusicTimeLength(musicArray[currentMusic]);
            SeekMusicStream(musicArray[currentMusic], length * percent);
        }
    }
 
    void Update()
    {
        if (!musicArray.empty())
        {
            UpdateMusicStream(musicArray[currentMusic]);

            float timePlayed = GetMusicTimePlayed(musicArray[currentMusic]);
            float timeLength = GetMusicTimeLength(musicArray[currentMusic]);

            if (timePlayed >= timeLength - 0.1f)
            {
                Next();
            }
        }
    }
    void DrawMusicList()
    {
        int y = 100;
        int maxView = 15;

        if (scrolling > (int)musicNames.size() - maxView)
        {
            scrolling = std::max(0, (int)musicNames.size() - maxView);
        }if (scrolling < 0)
        {
            scrolling = 0;
        }
        int start = (int)scrolling;
        for (size_t i = start; i <std::min(start + maxView, (int)musicNames.size()); i++)
        {
           
            Color color = (i == currentMusic) ? RED : BLACK;
   
            DrawText( musicNames[i].c_str(),  50, (float)y, 20, color);

            y += 25;
      
        }
    }
    void CheckSongClick()
    {
        Vector2 mouse = GetMousePosition();
        int y = 100;
        int maxView = 15;

        int start = (int)scrolling;
        int end = std::min(start + maxView, (int)musicNames.size());

        for (int i = start; i < end; i++)
        {
            Rectangle rect = { 50, (float)y, 800, 25 }; 
            if (CheckCollisionPointRec(mouse, rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                SelectSong(i);
            }
            y += 25;
            DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, GRAY);
        }
    }
    float GetProgress()
    {
        if (!musicArray.empty())
        {
            return GetMusicTimePlayed(musicArray[currentMusic]) / GetMusicTimeLength(musicArray[currentMusic]);
        }
        return 0.0f;
    }

    float GetSpeed() { return speed; }

    float GetCurrentTime()
    {
        if (!musicArray.empty())
        {
            return GetMusicTimePlayed(musicArray[currentMusic]);
        }
        return 0.0f;
    }

    float GetTotalTime()
    {
        if (!musicArray.empty())
        {
            return GetMusicTimeLength(musicArray[currentMusic]);
        }
        return 0.0f;
    }
    Music GetCurrentMusic()
    {
        return musicArray[currentMusic];
    }
    void SelectSong(int index)
    {

        if (index >= 0 && index < musicArray.size())
        {
            StopMusicStream(musicArray[currentMusic]);
            currentMusic = index;
            PlayMusicStream(musicArray[currentMusic]);
            isPaused = false;

            SetMusicPitch(musicArray[currentMusic], speed);

        }
    }
};
