#define is_down(b) (input->buttons[b].is_down)
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_1_p; //Player 1 position on the y-axis
float player_1_dp; //Player 1 velocity on the y-axis
float player_2_p; //Player 2 position on the y-axis
float player_2_dp; //Player 2 velocity on the y-axis

//Arena sizing
float arena_half_size_x = 80;
float arena_half_size_y = 45;

//Player Sizing
float player_half_size_x = 2.5;
float player_half_size_y = 12;

int player_1_score, player_2_score;

//Ball positioning, sizing, and velocity
float ball_p_x, ball_p_y, ball_dp_x = 130, ball_dp_y, ball_half_size = 1;

//Player movement and collision
internal void simulate_player(float *p, float *dp, float ddp, float dt)
{
	ddp -= *dp * 10.f; //friction effect to slow down paddles

	*p = *p + *dp * dt + ddp * dt * dt * 0.5f; //Update the player's position using the kinematic equation
	*dp = *dp + ddp * dt; //Update the player's velocity using the kinematic equation

	//Ensure player doesnt go beyond arena boundaries
	if (*p + player_half_size_y > arena_half_size_y)
	{
		*p = arena_half_size_y - player_half_size_y;
		*dp = 0;
	}
	else if (*p - player_half_size_y < -arena_half_size_y)
	{
		*p = -arena_half_size_y + player_half_size_y;
		*dp = 0;
	}
}

//axis-aligned bounding box collision between ball and paddles
internal bool aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y, float p2x, float p2y, float hs2x, float hs2y)
{
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y + hs1y < p2y + hs2y);
}


internal void simulate_game(Input *input, float dt)
{
	clear_screen(0x00000); //clears the screen with a black color
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x00000); //draws a rectangle as the background with color black

	//Player 1 acceleration on the y-axis
	float player_1_ddp = 0.f;
# if 0
	if (is_down(BUTTON_UP)) { player_1_ddp += 2000;}
	if (is_down(BUTTON_DOWN)) { player_1_ddp -= 2000;}
# else
	//if (ball_p_y > player_1_p) player_1_ddp += 1300;
	//if (ball_p_y < player_1_p) player_1_ddp -= 1300;
	player_1_ddp = (ball_p_y - player_1_p) * 100;
	if (player_1_ddp > 1300) player_1_ddp = 1300;
	if (player_1_ddp < -1300) player_1_ddp = -1300;
#endif


	float player_2_ddp = 0.f;

	if (is_down(BUTTON_W)) { player_2_ddp += 2000; }
	if (is_down(BUTTON_S)) { player_2_ddp -= 2000; }

	simulate_player(&player_1_p, &player_1_dp, player_1_ddp, dt);
	simulate_player(&player_2_p, &player_2_dp, player_2_ddp, dt);

	//Simulate Ball
	{
		ball_p_x += ball_dp_x * dt;
		ball_p_y += ball_dp_y * dt;

		if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 80, player_1_p, player_half_size_x, player_half_size_y))
		{
			ball_p_x = 80 - player_half_size_x - ball_half_size;
			ball_dp_x *= -1;
			ball_dp_y = (ball_p_y - player_1_p) * 2 + player_1_dp * 0.75f;
		}
		else if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, -80, player_2_p, player_half_size_x, player_half_size_y))
		{
			ball_p_x = -80 + player_half_size_x + ball_half_size;
			ball_dp_x *= -1;
			ball_dp_y = (ball_p_y - player_2_p) * 2 + player_2_dp * 0.75f;
			//if(ball_dp_y == 0) { rand() % 21 - 10; }
		}

		if (ball_p_y + ball_half_size > arena_half_size_y)
		{
			ball_p_y = arena_half_size_y - ball_half_size;
			ball_dp_y *= -1;
		}
		else if (ball_p_y - ball_half_size < -arena_half_size_y)
		{
			ball_p_y = -arena_half_size_y + ball_half_size;
			ball_dp_y *= -1;
		}

		if (ball_p_x + ball_half_size > arena_half_size_x)
		{
			ball_dp_x *= -1;
			ball_dp_y = 0;
			ball_p_x = 0;
			ball_p_y = 0;
			player_1_score++;
		}
		else if (ball_p_x - ball_half_size < -arena_half_size_x)
		{
			ball_dp_x *= -1;
			ball_dp_y = 0;
			ball_p_x = 0;
			ball_p_y = 0;
			player_2_score++;
		}
	}

	draw_number(player_1_score, -10, 40, 1.f, 0xbbffbb);
	draw_number(player_2_score, 10, 40, 1.f, 0xbbffbb);


	//Rendering
	draw_rect(ball_p_x, ball_p_y, ball_half_size, 1, 0x0FF00); //ball, color green

	draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, 0xff0000); //Player 1 paddle, color red
	draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, 0xff0000); //Player 2 paddle, color red
}