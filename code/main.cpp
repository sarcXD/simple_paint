
#include "raylib.h"
#include <math.h>

/**
 * TODO(talha)
 * Understanding how texture rendering works:
 * * Look into glfw since raylib uses glfw for any subsequent
 * * drawing as a means of using opengl
 */
#define Abs(x) (x < 0 ? x*-1: x)

const int screenWidth = 800;
const int screenHeight = 450;
const int horizontalPadding = 40;
const int verticalPadding = 40;
const int drawWidth = screenWidth - horizontalPadding;
const int drawHeight = screenHeight - verticalPadding;
Vector2 lastMousePosition;
Vector2 currentMousePosition;

void InsertNewPosition(Vector2 MousePos) {
	if (lastMousePosition.x == 0.0f && lastMousePosition.y == 0.0f) {
		lastMousePosition = MousePos;
	}
	currentMousePosition = MousePos;
}

bool IsEmpty(Vector2 Point) {
	return Point.x == 0 && Point.y == 0;
}

/**
 * NOTE(Talha) DDA Line Drawing Algorithm, SIMPLE VERSION
 * compute dy, dx
 * compute number of steps required for placing pixels:
 * -> MAX(ABS(dy), ABS(dx))
 * compute the increment for each coordinate
 * -> x_inc = dx/steps;
 * -> y_inc = dy/steps;
 * do
 * x_k+1 = x_k (starting point) + x_inc
 * y_k+1 = y_k (starting point) + y_inc
 * draw a pixel at point (x_k+1, y_k+1)
 * until run steps times i.e ${iterator} == steps
 */
void DDALineDrawing(Vector2 startPoint, Vector2 endPoint, RenderTexture2D *drawBoard) {
	float dy = endPoint.y - startPoint.y;
	float dx = endPoint.x - startPoint.x;
	float x = startPoint.x;
	float y = startPoint.y;
	float y_k, x_k;
	int steps = Abs(dy) > Abs(dx) ? Abs(dy) : Abs(dx);
	float incX = dx / (float) steps;
	float incY = dy / (float) steps;
	int step = 0;
	BeginTextureMode(*drawBoard);	
	do {
		y_k = y + incY;
		x_k = x + incX;
		y = y_k;
		x = x_k;
		DrawPixel(round(x)-horizontalPadding/2, round(y)-verticalPadding/2, BLACK);
		step++;
	} while (step < steps);
	EndTextureMode();	
}

void DrawLinedPixels(RenderTexture2D *drawBoard) {
	if (IsMouseButtonDown(0)) {
		Vector2 MousePos = GetMousePosition();
		InsertNewPosition(MousePos);
	}
	if (IsMouseButtonUp(0)) {
		currentMousePosition.x = 0;
		currentMousePosition.y = 0;
		lastMousePosition = currentMousePosition;
	}
	if (IsEmpty(lastMousePosition) != true && IsEmpty(currentMousePosition) != true) {
		// Note(Talha): This function represents my line implementation using DDA
		DDALineDrawing(lastMousePosition, currentMousePosition, drawBoard);
	}
	lastMousePosition = currentMousePosition;
}

void EmptyDrawingBoard(RenderTexture2D *drawBoard) {
	if (IsMouseButtonPressed(1)) {
		BeginTextureMode(*drawBoard);
		ClearBackground(WHITE);
		EndTextureMode();
	}
}

int main()
{
	// Initialization
	//--------------------------------------------------------------------------------------
	InitWindow(screenWidth, screenHeight, "raylib game");
	// TODO: Load resources / Initialize variables at this point
	SetTargetFPS(120);
	//--------------------------------------------------------------------------------------
	// Main game loop
	// loads texture for rendering (apparently a framebuffer)
	RenderTexture2D drawBoard = LoadRenderTexture(drawWidth, drawHeight);
	// clear render texture before entering game loop
	// NOTE: WHat is a render texture?
	BeginTextureMode(drawBoard); // Begins drawing to render texture
	ClearBackground(WHITE);
	EndTextureMode(); // ends drawing to render texture
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update variables / Implement example logic at this point
		//----------------------------------------------------------------------------------
		EmptyDrawingBoard(&drawBoard);
		DrawLinedPixels(&drawBoard);
		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		ClearBackground(DARKGRAY);
		// TODO: Draw everything that requires to be drawn at this point:
		DrawFPS(0, 0);
		Rectangle flippedBoard = { 0, 0, (float) (drawBoard.texture.width), (float) -(drawBoard.texture.height) };
		Vector2 texturePos = {horizontalPadding/2, verticalPadding/2};
		DrawTextureRec(drawBoard.texture, flippedBoard, texturePos, WHITE);
		EndDrawing();
		//----------------------------------------------------------------------------------
	}
	
	// De-Initialization
	//--------------------------------------------------------------------------------------
	// TODO: Unload all loaded resources at this point
	UnloadRenderTexture(drawBoard);
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------
	return 0;
}