// Second Verilog test -- 4 bit counter


module counter4Bit(clock,reset,enable,,counter_out);

//------------IO---------
input clock;
input reset;
input enable;
output [3:0] counter out // setting up a 4 bit array?

wire clock; //how specify specific connection?
wire reset;
wire enable;

reg [3:0] counter_out; //declaring a register -- RTL


//========= CODE STARTS HERE ============

always @ (posedge clock) //begin at rising edge
begin : COUNTER // Block Name
	if (reset == 1'b1) begin
		counter_out <= #1 4'b0000; // if reset is active, set output to 0000
	end

	else if (enable == 1'b1) begin
		counter_out <= #1 counter_out +1;
		//if enabled and reset is off, add to counter
	end

end //COUNTER BLOCK

endmodule

