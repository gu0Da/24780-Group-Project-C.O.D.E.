#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "yssimplesound.h"
#include "yspng.h"
#include "fssimplewindow.h"

int main(void)
{
	int x = 300, y = 700;
	int tx = 400, ty = 200;
	YsRawPngDecoder png[3];
	GLuint texId[3];
	FsOpenWindow(16, 16, 600, 800, 1);
	FsChangeToProgramDir();
	png[0].Decode("map.png");
	png[1].Decode("ep_12.png");
	png[2].Decode("boss_2.png");
	glGenTextures(1, &texId[0]);  // Reserve one texture identifier
	glBindTexture(GL_TEXTURE_2D, texId[0]);  // Making the texture identifier current (or bring it to the deck)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D
	(GL_TEXTURE_2D,
		0,    // Level of detail
		GL_RGBA,
		png[0].wid,
		png[0].hei,
		0,    // Border width, but not supported and needs to be 0.
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		png[0].rgba);

	glGenTextures(1, &texId[1]);  // Reserve one texture identifier
	glBindTexture(GL_TEXTURE_2D, texId[1]);  // Making the texture identifier current (or bring it to the deck)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D
	(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		png[1].wid,
		png[1].hei,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		png[1].rgba);

	glGenTextures(1, &texId[2]);  // Reserve one texture identifier
	glBindTexture(GL_TEXTURE_2D, texId[2]);  // Making the texture identifier current (or bring it to the deck)

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D
	(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		png[2].wid,
		png[2].hei,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		png[2].rgba);

	for (;;)
	{
		FsPollDevice();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glColor4d(1.0, 1.0, 1.0, 1.0);

		glEnable(GL_TEXTURE_2D);  // Begin using texture mapping
		//Draw Background
		glBindTexture(GL_TEXTURE_2D, texId[0]);

		glBegin(GL_QUADS);

		glTexCoord2d(0.0, 0.0);
		glVertex2i(0, 0);

		glTexCoord2d(1.0, 0.0);
		glVertex2i(600, 0);

		glTexCoord2d(1.0, 1.0);
		glVertex2i(600, 800);

		glTexCoord2d(0.0, 1.0);
		glVertex2i(0, 800);

		glEnd();

		glDisable(GL_TEXTURE_2D);  // End using texture mapping

		if (0 != FsGetKeyState(FSKEY_ESC))
		{
			break;
		}
		if (0 != FsGetKeyState(FSKEY_LEFT))
		{
			x -= 10;
			if (x <= 0)
			{
				x = 795;
			}
		}
		if (0 != FsGetKeyState(FSKEY_RIGHT))
		{
			x += 10;
			if (x >= 800)
			{
				x = 0;
			}
		}
		glEnable(GL_TEXTURE_2D);  // Begin using texture mapping
		glBindTexture(GL_TEXTURE_2D, texId[2]);

		glBegin(GL_QUADS);

		glTexCoord2d(0.0, 0.0);
		glVertex2i(tx - 92, ty - 118);

		glTexCoord2d(1.0, 0.0);
		glVertex2i(tx + 92, ty - 118);

		glTexCoord2d(1.0, 1.0);
		glVertex2i(tx + 92, ty + 118);

		glTexCoord2d(0.0, 1.0);
		glVertex2i(tx - 92, ty + 118);

		glEnd();

		glBindTexture(GL_TEXTURE_2D, texId[1]);

		glBegin(GL_QUADS);

		glTexCoord2d(0.0, 0.0);   // For each vertex, assign texture coordinate before vertex coordinate.
		glVertex2d(x - 20.0, y - 20.0);

		glTexCoord2d(1.0, 0.0);
		glVertex2d(x + 20.0, y - 20.0);

		glTexCoord2d(1.0, 1.0);
		glVertex2d(x + 20.0, y + 20.0);

		glTexCoord2d(0.0, 1.0);
		glVertex2d(x - 20.0, y + 20.0);

		glEnd();
		glDisable(GL_TEXTURE_2D);
		glFlush();
		FsSwapBuffers();
		FsSleep(10);
	}
	return 0;
}