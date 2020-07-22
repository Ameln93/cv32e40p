////////////////////////////////////////////////////////////////////////////////
//
// Filename:  obi_arbiter.v
//
// Purpose: This is a priority bus arbiter.  It allows two separate obi
//    masters to connect to the same bus, while also guaranteeing
//  that the last master can have the bus with no delay any time it is
//  idle.
//  The core logic works like this:
//
//    1. If 'A' or 'B' asserts the request line, a pending transaction begins
//    2. If both 'A' and 'B' assert req at the same time, only 'A'
//      will be granted the bus.
//    3. The bus will remain owned by whomever the bus was granted to
//      until the rvalid is asserted
//    4. Only one outstanding transfer is supported. Otherwise we would have
//      to count the number of outstanding transactions.
//
// Creator: Matthias von Ameln
//          TU Dresden
//
////////////////////////////////////////////////////////////////////////////////
module obi_arbiter
(
  input logic  clk_i,
  input logic  rst_ni,

  // input slave a
  input  logic        s_req_a_i,
  input  logic [31:0] s_addr_a_i,
  input  logic [ 3:0] s_be_a_i,
  input  logic [ 3:0] s_we_a_i,
  input  logic [31:0] s_wdata_a_i,

  output logic        s_gnt_a_o,
  output logic        s_rvalid_a_o,
  output logic [31:0] s_rdata_a_o,

  // input slabe b
  input  logic        s_req_b_i,
  input  logic [31:0] s_addr_b_i,
  input  logic [ 3:0] s_be_b_i,
  input  logic [ 3:0] s_we_b_i,
  input  logic [31:0] s_wdata_b_i,

  output logic        s_gnt_b_o,
  output logic        s_rvalid_b_o,
  output logic [31:0] s_rdata_b_o,

  // output master
  output logic        m_req_o,
  output logic [31:0] m_addr_o,
  output logic [ 3:0] m_be_o,
  output logic [ 3:0] m_we_o,
  output logic [31:0] m_wdata_o,

  input  logic        m_gnt_i,
  input  logic        m_rvalid_i,
  input  logic [31:0] m_rdata_i
);

typedef enum logic[2:0] { IDLE, REQ_A_PEND, REQ_B_PEND} arbiter_fsm_e;
arbiter_fsm_e state_q, state_n;
logic a_owner;

always_comb begin

  state_n = state_q;
  a_owner = '1;

  case (state_q)

    IDLE: begin

      // prefer a over b
      if (s_req_a_i == '1) begin
        state_n = REQ_A_PEND;
        a_owner = '1;
      end else if (s_req_b_i == '1) begin
        state_n = REQ_B_PEND;
        a_owner = '0;
      end
    end

    REQ_A_PEND, REQ_B_PEND: begin

      // if slave b owns the channel it must have it until the end of the
      // cycle to complete the obi transfer
      a_owner = (state_q == REQ_A_PEND) ? '1 : '0;

      if      (m_rvalid_i == '1 && s_req_a_i == '1) state_n = REQ_A_PEND;
      else if (m_rvalid_i == '1 && s_req_b_i == '1) state_n = REQ_B_PEND;
      else if (m_rvalid_i == '1)                    state_n = IDLE;
    end

    default: begin
      state_n = state_q;
      a_owner = '1;
    end

  endcase

end

  always_ff @(posedge clk_i) begin
    if (rst_ni == '0) begin
      state_q <= IDLE;
    end else begin
      state_q <= state_n;
    end
  end

  // We cannot allow the response valid to ever go high if
  // the slave in question does not own the bus.
  // Hence we force it low if the particular slave doesn't own the bus.
  assign s_rvalid_a_o = ( a_owner) ? m_rvalid_i : '0;
  assign s_rvalid_b_o = (!a_owner) ? m_rvalid_i : '0;

  assign s_rdata_a_o = ( a_owner) ? m_rdata_i : '0;
  assign s_rdata_b_o = (!a_owner) ? m_rdata_i : '0;

  // Grant of slave interface must be low, if the bus isn't granted to him.
  assign s_gnt_a_o  = ( a_owner) ? m_gnt_i : '0;
  assign s_gnt_b_o  = (!a_owner) ? m_gnt_i : '0;

  assign m_req_o    = (a_owner) ? s_req_a_i     : s_req_b_i;
  assign m_addr_o   = (a_owner) ? s_addr_a_i    : s_addr_b_i;
  assign m_be_o     = (a_owner) ? s_be_a_i      : s_be_b_i;
  assign m_we_o     = (a_owner) ? s_we_a_i      : s_we_b_i;
  assign m_wdata_o  = (a_owner) ? s_wdata_a_i   : s_wdata_b_i;

  //----------------------------------------------------------------------------
  // Assertions
  // (not supported by vivado 2018.3)
  // uncomment for 2020.1
  //----------------------------------------------------------------------------
  // check multiplication result for mulh
  //`ifndef VERILATOR
  // When master handshake signal is high a slave signal must also be high
  //assert property(@(posedge(clk) (m_req     && !s_req_a     && !s_req_b)));
  //assert property(@(posedge(clk) (m_gnt     && !s_gnt_a     && !s_gnt_b)));
  //assert property(@(posedge(clk) (m_rvalid  && !s_rvalid_a  && !s_rvalid_b)));

  // Maximal one slave can get an grant or response valid
  //assert property(@(posedge(clk) not (s_gnt_a && s_gnt_b)));
  //assert property(@(posedge(clk) not (s_rvalid_a && s_rvalid_b)));
  //`endif

  endmodule
