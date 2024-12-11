// IMPORTS SECTION
#include <stdint.h> // NOTE:Standard library for fixed-width integer types
#include <stdbool.h> //NOTE:  Standard library for boolean type
#include "oledDriver/oledC.h" // NOTE: OLED driver header
#include "oledDriver/oledC_colors.h"//NOTE: OLED color definitions
#include "oledDriver/oledC_shapeHandler.h" // NOTE: OLED shape handler header
#include "oledDriver/oledC_shapes.h" //NOTE: OLED shapes header

// Static variable to track if the example setup has been initialized
static bool exampleInitialized;

// Static variable to hold the background color for the OLED screen
static uint16_t background_color;

// Array defining a bitmap logo (23 rows of 32-bit binary data for the image)- NOTE: the amount of 1 and 0 controls the rendered logo shape.
static const uint32_t logo[] = 
{ 
    0b11111111111000000000011111111111,
    0b11111111100000000000001111111111,
    0b11111111000000000000000111111111,
    0b11111110000000000000000111111111,
    0b11111100000000000000000011111111,
    0b11111000000010000000100011111111,
    0b11111000000110000001100001111111,
    0b11110000001111000011110000111111,
    0b11110000001111100011111000111111,
    0b11110000000111100001111000011111,
    0b11110000000111110001111100001111,
    0b11110001000011110000111110001111,
    0b11110001100011111000111110001111,
    0b11110011110001111100011111001111,
    0b11110111110000111100001111001111,
    0b11111111100000111100001111111111,
    0b11111111100000011100000111111111,
    0b11111111000000011000000011111111,
    0b11111110000000000000000011111111,
    0b11111110000000000000000001111111,
    0b11111110000000000000000001111111,
    0b11111111100000000000000111111111,
    0b11111111111000000000011111111111
};

// Function to clear the OLED screen by filling it with the background color
static void oledC_clearScreen(void) 
{    
    uint8_t x; // Column index
    uint8_t y; // Row index
    oledC_setColumnAddressBounds(0,96); // Set the column bounds for clearing
    oledC_setRowAddressBounds(0,96); // Set the row bounds for clearing
    for(x = 0; x < 96; x++) // Iterate through columns
    {
        for(y = 0; y < 96; y++) // Iterate through rows
        {
            oledC_sendColorInt(background_color); // Fill each pixel with the background color
        }
    }
}

// Function to set the background color and clear the OLED screen
static void oledC_setBackground(uint16_t color)
{
    background_color = color; // Update the background color variable
    oledC_clearScreen(); // Clear the screen with the new background color
}

// Function to set up an example display with shapes and a bitmap logo
static void oledC_example_setup(void)
{
    oledC_setBackground(OLEDC_COLOR_WHITE); // Set the background to white
    shape_params_t params; // Structure to hold shape parameters
    
    // Configure parameters for the bitmap logo
    params.bitmap.color = OLEDC_COLOR_RED; // Set the logo color to red
    params.bitmap.x = 14; // Set the X-coordinate for the logo
    params.bitmap.y = 25; // Set the Y-coordinate for the logo
    params.bitmap.sx = 2; // Set horizontal scaling factor
    params.bitmap.sy = 2; // Set vertical scaling factor
    params.bitmap.bit_array = (uint32_t *)&logo[0]; // Pointer to the logo bitmap array
    params.bitmap.array_length = 23; // Number of rows in the bitmap
    oledC_addShape(0,OLED_SHAPE_BITMAP, &params); // Add the bitmap as a shape to the display
    
    // Configure and add a circle at (10, 10)
    params.circle.radius = 10; // Circle radius
    params.circle.xc = 10; // X-coordinate of the circle center
    params.circle.yc = 10; // Y-coordinate of the circle center
    oledC_addShape(1,OLED_SHAPE_CIRCLE, &params); // Add the circle shape
    
    // Configure and add a blue circle at (10, 85)
    params.circle.color = OLEDC_COLOR_BLUE; // Circle color
    params.circle.yc = 85; // Update Y-coordinate of the circle center
    oledC_addShape(2,OLED_SHAPE_CIRCLE, &params); // Add the circle shape
    
    // Configure and add a yellow circle at (85, 85)
    params.circle.color = OLEDC_COLOR_YELLOW; // Circle color
    params.circle.xc = 85; // Update X-coordinate of the circle center
    oledC_addShape(3,OLED_SHAPE_CIRCLE, &params); // Add the circle shape
    
    // Configure and add a green circle at (85, 10)
    params.circle.color = OLEDC_COLOR_GREEN; // Circle color
    params.circle.yc = 10; // Update Y-coordinate of the circle center
    oledC_addShape(4,OLED_SHAPE_CIRCLE, &params); // Add the circle shape
    
    oledC_redrawAll(); // Redraw all shapes on the screen
    exampleInitialized = true; // Mark the example as initialized
}

// Function to run the OLED example and animate the bitmap
void oledC_example(void)
{
    static int8_t shift = -24; // Shift amount for the bitmap's X-coordinate
    const uint8_t shift_from = 15; // Starting position for the animation
    if(!exampleInitialized) // Check if the example setup is not initialized
    {
        oledC_example_setup(); // Initialize the example setup
    }
    
    oledC_DrawString(10,0,2,2,(uint8_t*)"yotam", OLEDC_COLOR_BLACK); // Draw the string "yotam" at the top left
    oledC_DrawString(30,78,2,2,(uint8_t*)"Lab", OLEDC_COLOR_DARKGREEN); // Draw the string "Lab" at the bottom center
    shape_t *moveIt = oledC_getShape(0); // Get the bitmap shape to animate
           
    oledC_eraseShape(0, background_color); // Erase the current bitmap position
    moveIt->params.bitmap.x = shift_from + shift; // Update the X-coordinate with the shift
    shift += 4; // Increment the shift
    if(shift > 24) // Reset shift when it exceeds the bounds
    {
        shift = -24;
    }
    oledC_redrawIndex(0); // Redraw the bitmap at the new position
}
