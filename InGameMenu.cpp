#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include <stdio.h>
#include "yspng.h"
#include "InGameMenu.h"

TextString::TextString()
{
    str = nullptr;
}
TextString::TextString(const TextString& incoming)
{
    str = nullptr;
    CopyFrom(incoming);
}
TextString::~TextString()
{
    CleanUp();
}

TextString& TextString::operator=(const TextString& incoming)
{
    CopyFrom(incoming);
    return *this;
}

void TextString::CopyFrom(const TextString& incoming)
{
    Set(incoming.GetPointer());
}

void TextString::CleanUp(void)
{
    if (nullptr != str)
    {
        delete[] str;
        str = nullptr;
    }
}

void TextString::Add(char c)
{
    if (nullptr == str)
    {
        const char newStr[2] = { c,0 };
        Set(newStr);
    }
    else
    {
        // Allocate
        auto l = strlen(str);
        char* newStr = new char[l + 2];

        // Copy
        strcpy(newStr, str);
        newStr[l] = c;
        newStr[l + 1] = 0;

        // Set(newStr);
        // delete [] newStr;

        // CleanUp
        CleanUp();

        // Re-point
        str = newStr;
    }
}

void TextString::Add(const char str[])
{
    for (int i = 0; str[i] != 0; ++i)
    {
        Add(str[i]);
    }
}

void TextString::Set(const char incoming[])
{
    if (incoming != str)
    {
        auto l = strlen(incoming);
        char* newStr = new char[l + 1];
        strcpy(newStr, incoming);

        CleanUp();

        str = newStr;
    }
}

void TextString::BackSpace(void)
{
    if (nullptr != str)
    {
        auto l = strlen(str);

        if (0 < l)
        {
            str[l - 1] = 0;

            // Allocate
            char* newStr = new char[l];

            // Copy
            strcpy(newStr, str);

            // CleanUp
            CleanUp();

            // Re-point
            str = newStr;
        }
    }
}

const char* TextString::GetPointer(void) const
{
    if (nullptr != str)
    {
        return str;
    }
    else
    {
        return "";
    }
}

int TextString::Strlen(void) const
{
    return strlen(GetPointer());
}

const char* TextString::Fgets(FILE* fp)
{
    CleanUp();

    char str[16];
    for (;;)
    {
        if (nullptr != fgets(str, 15, fp))
        {
            for (int i = 0; str[i] != 0; ++i)
            {
                if (str[i] == '\n')
                {
                    return GetPointer();
                }
                Add(str[i]);
            }
        }
        else
        {
            if (0 < Strlen())
            {
                return GetPointer();
            }
            else
            {
                return nullptr;
            }
        }
    }
}

void InGameMenu::AddText(int level, int score, int health, char* weapon)
{
    showlevel.CleanUp();
    showweapon.CleanUp();
    showscore.CleanUp();
    showlevel.Add("Live: ");
    showlevel.Add(level + '0');
    showscore.Add("Score: ");
    showscore.Add(score + '0');
    showweapon.Add("Weapon: ");
    showweapon.Add(weapon);
}

void InGameMenu::Display(int level, int score, int health, char* weapon)
{
    //char pattern[] =
    //{
    //	"..11...11.."
    //	".1111.1111."
    //	"11111111111"
    //	"11111111111"
    //	".111111111."
    //	"..1111111.."
    //	"...11111..."
    //	"....111...."
    //	".....1....."
    //};
    char pattern[] =
    {
        "...11......11..."
        "..1111....1111.."
        ".111111..111111."
        "1111111111111111"
        "1111111111111111"
        ".11111111111111."
        "..111111111111.."
        "...1111111111..."
        "....11111111...."
        ".....111111....."
        "......1111......"
        ".......11......."
        "................"
        /*"................"
        "................"
        "................"*/
    };


    AddText(level, score, health, weapon);

    glColor3ub(255, 128, 0);
    glRasterPos2d(15, 60);
    YsGlDrawFontBitmap16x20(showlevel.GetPointer());
    glRasterPos2d(15, 90);
    YsGlDrawFontBitmap16x20(showweapon.GetPointer());
    glRasterPos2d(15, 120);
    YsGlDrawFontBitmap16x20(showscore.GetPointer());

    if (level == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f(0.0, 0, 0);
        glRasterPos2d(200, 350);
        YsGlDrawFontBitmap20x32("You Lose!");
        glRasterPos2d(180, 420);
        YsGlDrawFontBitmap20x32("Play Again?");
    }
    if (health >= 1)
    {
        glBegin(GL_QUADS);
        for (int y = 0;y < 16;y++)
        {
            for (int x = 0;x < 16;x++)
            {
                if (pattern[y * 16 + x] == '1')
                {
                    glVertex2i(x * 2 + 10, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 10, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 10, y * 2 + 5 + 130);
                    glVertex2i(x * 2 + 10, y * 2 + 2 + 130);
                    glColor3f(1.0, 0.0, 0.0);
                }

            }
        }
        glEnd();
    }
    if (health >= 2)
    {
        glBegin(GL_QUADS);
        for (int y = 0;y < 16;y++)
        {
            for (int x = 0;x < 16;x++)
            {
                if (pattern[y * 16 + x] == '1')
                {
                    glVertex2i(x * 2 + 50, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 50, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 50, y * 2 + 5 + 130);
                    glVertex2i(x * 2 + 50, y * 2 + 2 + 130);
                    glColor3f(1.0, 0.0, 0.0);
                }

            }
        }
        glEnd();
    }
    if (health >= 3)
    {
        glBegin(GL_QUADS);
        for (int y = 0;y < 16;y++)
        {
            for (int x = 0;x < 16;x++)
            {
                if (pattern[y * 16 + x] == '1')
                {
                    glVertex2i(x * 2 + 90, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 90, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 90, y * 2 + 5 + 130);
                    glVertex2i(x * 2 + 90, y * 2 + 2 + 130);
                    glColor3f(1.0, 0.0, 0.0);
                }

            }
        }
        glEnd();
    }
    if (health >= 4)
    {
        glBegin(GL_QUADS);
        for (int y = 0;y < 16;y++)
        {
            for (int x = 0;x < 16;x++)
            {
                if (pattern[y * 16 + x] == '1')
                {
                    glVertex2i(x * 2 + 130, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 130, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 130, y * 2 + 5 + 130);
                    glVertex2i(x * 2 + 130, y * 2 + 2 + 130);
                    glColor3f(1.0, 0.0, 0.0);
                }

            }
        }
        glEnd();
    }
    if (health ==5)
    {
        glBegin(GL_QUADS);
        for (int y = 0;y < 16;y++)
        {
            for (int x = 0;x < 16;x++)
            {
                if (pattern[y * 16 + x] == '1')
                {
                    glVertex2i(x * 2 + 170, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 170, y * 2 + 130);
                    glVertex2i(x * 2 + 2 + 170, y * 2 + 5 + 130);
                    glVertex2i(x * 2 + 170, y * 2 + 2 + 130);
                    glColor3f(1.0, 0.0, 0.0);
                }

            }
        }
        glEnd();
    }
}
