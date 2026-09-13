#include <hardware/pwm.h>
#include <lwip/tcp.h>
#include <lwip/tcpbase.h>
#include <lwipopts.h>
#include <pico/cyw43_arch.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "ac.h"
#include "webpage.h"

#define CMD_NULL 0x0
#define CMD_TOGGLE 0x1
#define CMD_DOWN 0x2
#define CMD_UP 0x3

static const char* header =
	"HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";

int parse_route(char* route) {
	if (strncmp(route, "GET /tog", 8) == 0)
		return CMD_TOGGLE;
	if (strncmp(route, "GET /inc", 8) == 0)
		return CMD_UP;
	if (strncmp(route, "GET /dec", 8) == 0)
		return CMD_DOWN;

	return CMD_NULL;
}

static err_t recv_cb(void* arg, struct tcp_pcb* pcb, struct pbuf* p, err_t e) {
	if (p == NULL) {
		tcp_close(pcb);
		return ERR_CLSD;
	}

	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

	// check route
	int cmd = parse_route((char*)p->payload);

	tcp_write(pcb, header, strlen(header), TCP_WRITE_FLAG_COPY);

	if (cmd == CMD_NULL) {
		tcp_write(pcb, webpage, strlen(webpage), TCP_WRITE_FLAG_COPY);
	} else if (cmd == CMD_TOGGLE) {
		tcp_write(pcb, "ok", 2, TCP_WRITE_FLAG_COPY);
		push_button(&s1);
	} else if (cmd == CMD_DOWN) {
		tcp_write(pcb, "ok", 2, TCP_WRITE_FLAG_COPY);
		push_button(&s2);
	} else if (cmd == CMD_UP) {
		tcp_write(pcb, "ok", 2, TCP_WRITE_FLAG_COPY);
		push_button(&s3);
	}

	tcp_output(pcb);
	tcp_recved(pcb, p->tot_len);
	pbuf_free(p);
	tcp_close(pcb);

	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

	return ERR_OK;
}

static err_t req_accept(void* arg, struct tcp_pcb* pcb, err_t t) {
	tcp_recv(pcb, recv_cb);
	return ERR_OK;
}

int main() {
	stdio_init_all();

	// Initialise the Wi-Fi chip
	if (cyw43_arch_init()) {
		printf("Wi-Fi init failed\n");
		return -1;
	}

	// led flash
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
	sleep_ms(100);
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

	cyw43_arch_enable_sta_mode();
	if (cyw43_arch_wifi_connect_timeout_ms("RAJNEW", "1133557799",
										   CYW43_AUTH_WPA2_AES_PSK, 8000)) {
	} else {
		cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
	}

	init_servos();

	struct tcp_pcb* pcb = tcp_new();
	tcp_bind(pcb, IP_ADDR_ANY, 80);
	pcb = tcp_listen(pcb);
	tcp_accept(pcb, req_accept);

	// led flash
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
	sleep_ms(100);
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

	while (1) {
		cyw43_arch_poll();
		sleep_ms(1);
	}

	return 0;
}
