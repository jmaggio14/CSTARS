//first verilog test -- Hello World
module hello_world;

initial begin 
	$display ("Hello World!");
	#10 $finish
end

endmodule 


