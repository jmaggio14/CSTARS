module kmk_star_fsm (
	resetn, clk, clk_fast,
	ld_y, ld_x, clk_x,
	addr,
	s, r, rst_y, cal,
	zybo2_clk, zybo2_fs,
	adc_clk );

`include "./kmk_star_fsm.h"

//fix "clk_fast", replace with clk with same
//period as ld_x or clk_x

input resetn, clk, clk_fast;

output reg ld_y, ld_x, clk_x, s, r, rst_y, cal;
output reg zybo2_fs;
output reg [0:9] addr;
output adc_clk;
output zybo2_clk;

reg [7:0] state;
reg [0:9] addr_store;

reg [7:0] count;
reg zybo2_clk_en;

assign adc_clk = clk_fast;
assign zybo2_clk = zybo2_clk_en ? ~clk_fast : 1'b0;

always @(posedge clk)
if (!resetn) begin
	ld_y<=0; ld_x<=0; clk_x<=0; s<=0; r<=0; rst_y<=0; cal<=0;
	zybo2_clk_en<=0;
	addr_store<=10'h000; addr<=10'h000;
end
else begin
	case (state)
		8'h00: begin
			//zybo2_clk_en <= 1'b0;
			addr <= addr_store;
			ld_y <= 1'b1;
			state <= state + 1'b1;
		end
		8'h01: begin
			ld_y <= 1'b0;
			state <= state + 1'b1;
		end
		8'h02: begin
			ld_y <= 1'b1;
			count <= `ADDR_Y_HOLD_TIME;
			state <= state + 1'b1;
		end
		8'h03: begin //wait for Y Address Hold Time
			if (count > 0)
				count <= count - 1;
			else begin
				s <= 1'b1;
				count <= `S_HOLD_TIME;
				state <= state + 1'b1;
			end
		end
		8'h04: begin //wait for S hold time
			if ((count == `CAL_BEFORE_S_END) & (addr == 10'h000))
				cal <= 1'b1;	
			if (count > 0)
				count <= count - 1;
			else begin
				s <= 1'b0;
				cal <= 1'b0;
				count <= `S_TO_RST_Y_TIME;
				state <= state + 1'b1;
			end
		end
		8'h05: begin
			if (count > 0)
				count <= count - 1;
			else begin
				rst_y <= 1'b1;
				count <= `RST_Y_HOLD_TIME;
				state <= state + 1'b1;
			end
		end
		8'h06: begin
			if (count > 0)
				count <= count - 1;
			else begin
				rst_y <= 1'b0;
				count <= `RST_Y_TO_R_TIME;
				state <= state + 1'b1;
			end
		end
		8'h07: begin
			if (count > 0)
				count <= count - 1;
			else begin
				r <= 1'b1;
				count <= `R_HOLD_TIME;
				state <= state + 1'b1;
			end
		end
		8'h08: begin
			if (count > 0)
				count <= count - 1;
			else begin
				r <= 1'b0;
				state <= state + 1'b1;
			end
		end
		//begin clocking X pixels
		8'h09: begin
			ld_x <= 1'b0;
			clk_x <= 1'b1;
			addr <= 10'h000;
			state <= state + 1'b1;
		end
		8'h0A: begin
			if (addr >= 10'h008)
				zybo2_clk_en <= 1'b1;
			state <= state + 1'b1;
		end
		8'h0B: begin
			if (addr != 10'h3FF) begin
				ld_x <= 1'b1;
				clk_x <= 1'b0;
				state <= state + 1'b1;
			end
			else begin
				ld_x <= 1'b1;
				clk_x <= 1'b0;
				state <= 8'h0D;
			end
		end
		8'h0C: begin
			ld_x <= 1'b0;
			clk_x <= 1'b1;
			addr <= addr + 1'b1;
			state <= 8'h0A;
		end
		//clock output 3 times more to pipe out
		//(once for addr 0x3ff, then 2 times more)
		8'h0D: begin
			clk_x <= 1'b1;
			state <= state + 1'b1;
		end
		8'h0E: begin
			state <= state + 1'b1;
		end
		8'h0F: begin
			clk_x <= 1'b0;
			state <= state + 1'b1;
		end
		8'h10: begin
			clk_x <= 1'b1;
			state <= state + 1'b1;
		end
		8'h11: begin
			state <= state + 1'b1;
		end
		8'h12: begin
			clk_x <= 1'b0;
			state <= state + 1'b1;
		end
		8'h13: begin
			clk_x <= 1'b1;
			state <= 8'h00;
			addr_store <= addr_store + 1'b1;
		end
	
		default: state <= 8'hFF;

	endcase

	//if not clocking X pixels, cycle clk_x due to reg leakage
	if (state < 8'h09)
		clk_x <= ~clk_x;

end	

always @(posedge clk_fast)
if (!resetn) begin
	zybo2_fs<=0;
end

endmodule
