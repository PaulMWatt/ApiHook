/// @file Test_WS2_32_hook.h
/// 
/// Verifies the unit-test hook object for Windows Sockets through the WS2_32_hook library.
/// 
/// The MIT License(MIT)
/// @copyright 2015 Paul M Watt
/// 
/// Verify data with these TEST ASSERTIONS:
/// 
///  TS_FAIL(message):                        Fail unconditionally
///  TS_ASSERT(expr):                         Verify (expr) is true
///  TS_ASSERT_EQUALS(x, y):                  Verify (x==y)
///  TS_ASSERT_SAME_DATA(x, y, size):         Verify two buffers are equal
///  TS_ASSERT_DELTA(x, y, d):                Verify (x==y) up to d
///  TS_ASSERT_DIFFERS(x, y):                 Verify !(x==y)
///  TS_ASSERT_LESS_THAN(x, y):               Verify (x<y)
///  TS_ASSERT_LESS_THAN_EQUALS(x, y):        Verify (x<=y)
///  TS_ASSERT_PREDICATE(P, x):               Verify P(x)
///  TS_ASSERT_RELATION(R, x, y):             Verify x R y, ex. TS_ASSERT_RELATION(std::greater, x, y);
///  TS_ASSERT_THROWS(expr, type):            Verify that (expr) throws a specific type of exception.
///  TS_ASSERT_THROWS_EQUALS(expr, arg, x, y):Verify type and value of what (expr) throws
///  TS_ASSERT_THROWS_ANYTHING(expr):         Verify that (expr) throws an exception
///  TS_ASSERT_THROWS_NOTHING(expr):          Verify that (expr) doesn't throw anything
///  TS_WARN(message):                        Print message as a warning
///  TS_TRACE(message):                       Print message as an information message
/// 
//  ****************************************************************************
#ifndef Test_WS2_32_hook_H_INCLUDED
#define Test_WS2_32_hook_H_INCLUDED
//  Includes *******************************************************************
#include <cxxtest/TestSuite.h>
#include "../../../../src/api/windows/WS2_32/WS2_32.h"

using namespace cxxhook;
using namespace cxxhook::ipc;

//  ****************************************************************************
/// Test_WS2_32_hook Test Suite class.
///
class Test_WS2_32_hook : public CxxTest::TestSuite
{
public:

  Test_WS2_32_hook()
  {
    // TODO: Construct Test Suite Object
  }

  // Fixture Management ********************************************************
  // setUp will be called before each test case in order to setup common fixtures.
  virtual void setUp()
  {
    sut = std::make_shared<SUT>();
    sut->hook();
  }
 
  // tearDown will be called after each test case to clean up common resources.
  virtual void tearDown()
  {
    sut->unhook();
  }

protected:
  // Test Suite Data ***********************************************************
  typedef cxxhook::WS2_32                                   SUT;
  std::shared_ptr<SUT>      sut;


  // Creator Methods ***********************************************************
  // TODO: Use creator methods to reduce redundant setup code in test cases.

public:
  // Test Cases ****************************************************************
  void Test_socket_tcp(void);
  void Test_socket_udp(void);
  void Test_socket_raw(void);

  void Test_closesocket_tcp(void);
  void Test_closesocket_udp(void);
  void Test_closesocket_no_socket(void);

  void Test_shutdown(void);
  void Test_shutdown_no_socket(void);


};

//  ****************************************************************************
void Test_WS2_32_hook::Test_socket_tcp(void)
{
  // SUT
  SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  // Verify
  TS_ASSERT(sock != 0);
  TS_ASSERT_DIFFERS((sock % 2), 0);

  TcpSocketSP tcpSock = sut->get_tcp_socket_state(sock);
  TS_ASSERT_EQUALS(!tcpSock, false);
}

//  ****************************************************************************
void Test_WS2_32_hook::Test_socket_udp(void)
{
  // SUT
  SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  // Verify
  TS_ASSERT(sock != 0);
  TS_ASSERT_EQUALS((sock % 2), 0);

  UdpSocketSP udpSock = sut->get_udp_socket_state(sock);
  TS_ASSERT_EQUALS(!udpSock, false);
}

//  ****************************************************************************
void Test_WS2_32_hook::Test_socket_raw(void)
{
  // SUT
  SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);

  // Verify
  TS_ASSERT_EQUALS(sock, -1);
  TS_ASSERT_EQUALS(WSA_INVALID_PARAMETER, WSAGetLastError());

  TcpSocketSP tcpSock = sut->get_tcp_socket_state(sock);
  TS_ASSERT(!tcpSock);
}

//  ****************************************************************************
void Test_WS2_32_hook::Test_closesocket_tcp(void)
{
  SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  // SUT
  int result = closesocket(sock);

  TS_ASSERT_EQUALS(result, 0);
}

//  ****************************************************************************
void Test_WS2_32_hook::Test_closesocket_udp(void)
{
  SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  // SUT
  int result = closesocket(sock);

  TS_ASSERT_EQUALS(result, 0);
}

//  ****************************************************************************
void Test_WS2_32_hook::Test_closesocket_no_socket(void)
{
  // SUT
  SOCKET sock = 12345;
  int result = closesocket(sock);

  TS_ASSERT_EQUALS(result, -1);
  TS_ASSERT_EQUALS(WSAGetLastError(), error::k_socketNotSocket);
}

//  ****************************************************************************
void Test_WS2_32_hook::Test_shutdown(void)
{
  SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  // SUT
  int result = shutdown(sock, SD_RECEIVE);
  TS_ASSERT_EQUALS(result, 0);

  result = shutdown(sock, SD_SEND);
  TS_ASSERT_EQUALS(result, 0);

  result = shutdown(sock, SD_BOTH);
  TS_ASSERT_EQUALS(result, 0);
}

//  ****************************************************************************
void Test_WS2_32_hook::Test_shutdown_no_socket(void)
{
  // SUT
  SOCKET sock = 321;
  int result  = shutdown(sock, SD_BOTH);

  TS_ASSERT_EQUALS(result, -1);
  TS_ASSERT_EQUALS(WSAGetLastError(), error::k_socketNotSocket);
}

#endif