#include <unistd.h>
#include <pthread.h>

#define PIXELS

#include <tegrine/instances.c>
#include <tegrine/term.c>
#include <tegrine/json.c>

struct {
	char c;

	Tegrine te;

	D2	cur,
		size;

	RGBA color;
} z = { };

char get_c() {
	px_assert(read(0, &z.c, 1) != -1, ERROR"read");

	return z.c;
}

void sub(int *x) {
	*x -= *x ? 1 : 0;
}

void add_l(int *x, int max) {
	*x += *x < max - 1 ? 1 : 0;
}

// Slow
size_t get_pixel_index_at_pos(D2 *add, Pixels *px, D2 *pos) {
	for(int i = 0; i < px->len; ++i) {
		Px *p = &px->x[i];

		if (p->pos.x + add->x == pos->x && p->pos.y + add->y == pos->y)
			return i;
	}

	return -1;
}

// Yes this is the whole game lol :3
void * game(void *_) {
	(void)_;

	for(;;) {
		if (!arc4random_uniform(6)) {
			Instance *ins = add_Instance(
				&z.te.x,
				&z.cur,
				&z.size
			);

			ins->pos = (D2){
				-9,
				arc4random_uniform(z.te.ws.y) - 35
			},
			ins->pixels = z.te.x.x[0].pixels;
		}

		for(int i = 0; i < z.te.x.len; ++i) {
			Instance *ins = &z.te.x.x[i];

			// Movement
			++ins->pos.x;

			// Collision
			if (get_pixel_index_at_pos(
				&ins->pos,
				&ins->pixels,
				&z.cur
			) != (size_t)-1)
				exit(69);
		}

		draw(&z.te, &z.cur),
		// There's also select/nanosleep both non-portable
		// TODO: Fix lag: It blocks syscalls :(
		usleep(50000);
	}
}

int main() {
	load_from_json(&z.te, "goldfish"),

	set_ws(&z.te.ws),
	term_raw();

	pthread_t th;

	// Start
	z.te.x.x[0].pos.x = -9,
	z.cur = (D2){ z.te.ws.x / 2, z.te.ws.y / 2 },
	pthread_create(&th, 0, game, 0);

	for(;;)
		switch (get_c()) {
			case 'h':
				sub(&z.cur.x);
				
				break;
			case 'k':
				sub(&z.cur.y);
				
				break;
			case 'j':
				add_l(&z.cur.y, z.te.ws.y);
				
				break;
			case 'l':
				add_l(&z.cur.x, z.te.ws.x);
				
				break;

			case 'q':
				exit(0);
		}
}
