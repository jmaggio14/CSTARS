include "counter.v"
module counter_tb();

reg clock,reset,enable;
wire[3:0] counter_out;


initial begin
	$display("time\t clk reset enable counter");
	$monitor("%g\t %b %b %b %b",
	$time, clock,reset,enable,counter_out);
	clock=;
