/*
    License for Butano:

    Copyright (c) 2020-2022 Gustavo Valiente gustavo.valiente@protonmail.com
    zlib License, see LICENSE file.
*/

// Butano libraries
#include "bn_core.h"
#include "bn_log.h"
#include "bn_sram.h"
#include "bn_music.h"
#include "bn_music_actions.h"
#include "bn_music_items.h"
#include "bn_sound_items.h"
#include "bn_math.h"
#include "bn_string.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_random.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_palette_ptr.h"
#include "common_info.h"
#include "common_variable_8x8_sprite_font.h"
#include "bn_sprite_items_paddle.h"
#include "bn_sprite_items_ball.h"
#include "bn_regular_bg_items_bg.h"

int main()
{
    bn::core::init();

    // Creates the background and sprites
    bn::regular_bg_ptr bg = bn::regular_bg_items::bg.create_bg(0, 0);
    bn::sprite_ptr paddle = bn::sprite_items::paddle.create_sprite(0, 60); // Moved paddle up slightly
    paddle.set_rotation_angle(90);

    bn::sprite_ptr ball = bn::sprite_items::ball.create_sprite(0, 0);

    // Initialize the game variables
    int score = 0;
    bn::fixed delta_x = 0;
    bn::fixed delta_y = 0;
    bool game_over = false;
    bn::random random;

    // Constants
    const int PADDLE_Y = 60;  // Fixed paddle Y position
    const int PADDLE_WIDTH = 32;  // Half width of paddle
    const int BALL_SIZE = 4;
    const int SCREEN_TOP = -70;
    const int SCREEN_LEFT = -120;
    const int SCREEN_RIGHT = 120;
    const int SCREEN_BOTTOM = 70;

    // Setup the text display
    bn::sprite_text_generator text_generator(common::variable_8x8_sprite_font);
    bn::vector<bn::sprite_ptr, 16> text_sprites;
    text_generator.generate(-6 * 16, -68, "(Press X to start)", text_sprites);

    // Vector in order to hold trace 
    bn::vector<bn::sprite_ptr, 32> trace_sprites;

    while(true)
    {
        // Paddle movement logic
        if(bn::keypad::left_held() && paddle.x() > SCREEN_LEFT)
        {
            paddle.set_x(paddle.x() - 2);
        }
        else if(bn::keypad::right_held() && paddle.x() < SCREEN_RIGHT)
        {
            paddle.set_x(paddle.x() + 2);
        }

        // Game over -> restart logic
        if(game_over && bn::keypad::r_pressed())
        {
            ball.set_position(0, 0);
            paddle.set_position(0, PADDLE_Y);
            delta_x = 0;
            delta_y = 0;
            score = 0;
            game_over = false;

            text_sprites.clear();
            text_generator.generate(-6 * 16, -68, "(Press X to start)", text_sprites);
        }

        // Start the game
        if(bn::keypad::a_pressed() && delta_x == 0 && delta_y == 0 && !game_over)
        {
            text_sprites.clear();
            bn::string<32> txt_score = "Score: " + bn::to_string<32>(score);
            text_generator.generate(-6 * 16, -68, txt_score, text_sprites);

            // Initialize ball movement with random direction but always moving up initially
            delta_x = (random.get_int() % 5) - 2;
            delta_y = -2; // Start moving up
            
            bn::sound_items::pong.play();
        }

        // Ball's movement and collision logic
        if(delta_x != 0 || delta_y != 0)
        {
            // Add a new dotted point to the trace at current ball position
            if (trace_sprites.size() >= 32)  // Prevents overflow by limiting trace length
            {
                // Shifts elements to the front
                for(int i = 1; i < trace_sprites.size(); ++i)
                {
                    trace_sprites[i - 1] = trace_sprites[i];
                }
                trace_sprites.pop_back();  
            }

            // Adds new trace point
            trace_sprites.push_back(bn::sprite_items::ball.create_sprite(ball.x(), ball.y()));

            // Updates the ball's position
            ball.set_x(ball.x() + delta_x);
            ball.set_y(ball.y() + delta_y);

            // Top wall collision logic
            if(ball.y() <= SCREEN_TOP)
            {
                ball.set_y(SCREEN_TOP);
                delta_y = -delta_y;
                bn::sound_items::pong.play();
            }

            // Side wall collisions logic
            if(ball.x() <= SCREEN_LEFT)
            {
                ball.set_x(SCREEN_LEFT);
                delta_x = -delta_x;
                bn::sound_items::pong.play();
            }
            else if(ball.x() >= SCREEN_RIGHT)
            {
                ball.set_x(SCREEN_RIGHT);
                delta_x = -delta_x;
                bn::sound_items::pong.play();
            }

            // Paddle collision logic
            if(ball.y() + BALL_SIZE / 2 >= paddle.y() - PADDLE_WIDTH / 2 && ball.y() - BALL_SIZE / 2 <= paddle.y() + PADDLE_WIDTH  / 2) 
            {
                // Checks if ball is within paddle's width
                if(ball.x() >= paddle.x() - PADDLE_WIDTH && ball.x() <= paddle.x() + PADDLE_WIDTH)
                {
                    // Bounces the ball off the top of the paddle
                    ball.set_y(paddle.y() - (PADDLE_WIDTH / 2) - (BALL_SIZE / 2)); 
                    delta_y = -delta_y;  

                    // Calculates where on the paddle the ball hit
                    bn::fixed hit_position = (ball.x() - paddle.x()) / PADDLE_WIDTH;

                    // Adjust the horizontal speed based on where the ball hits the paddle
                    delta_x = hit_position * 3;  

                    score++;
                    text_sprites.clear();
                    bn::string<32> txt_score = "Score: " + bn::to_string<32>(score);
                    text_generator.generate(-6 * 16, -68, txt_score, text_sprites);

                    bn::sound_items::pong.play();
                    
                    // Resets the trace after ball hits the paddle
                    trace_sprites.clear();
                }
            }

            // Ball goes past paddle -> game over
            if(ball.y() >= SCREEN_BOTTOM && !game_over)
            {
                game_over = true;
                ball.set_position(0, 0);
                paddle.set_position(0, PADDLE_Y);
                delta_x = 0;
                delta_y = 0;
                score = 0;
                
                text_sprites.clear();
                text_generator.generate(-6 * 16, -68, "Game Over! Press S to restart", text_sprites);

                // Clears trace on game over
                trace_sprites.clear();
            }
        }

        bn::core::update();
    }
}
