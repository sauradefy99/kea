// Copyright (C) 2013 Internet Systems Consortium, Inc. ("ISC")
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
// OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#ifndef PKT_FILTER_H
#define PKT_FILTER_H

#include <dhcp/pkt4.h>
#include <asiolink/io_address.h>
#include <boost/shared_ptr.hpp>

namespace isc {
namespace dhcp {

/// @brief Exception thrown when invalid packet filter object specified.
class InvalidPacketFilter : public Exception {
public:
    InvalidPacketFilter(const char* file, size_t line, const char* what) :
        isc::Exception(file, line, what) { };
};

/// Forward declaration to the structure describing a socket.
struct SocketInfo;

/// Forward declaration to the class representing interface
class Iface;

/// @brief Abstract packet handling class
///
/// This class represents low level method to send and receive DHCP packet.
/// Different methods, represented by classes derived from this class, use
/// different socket families and socket types. Also, various packet filtering
/// methods can be implemented by derived classes, e.g. Linux Packet
/// Filtering (LPF) or Berkeley Packet Filtering (BPF).
///
/// Low-level code operating on sockets may require special privileges to execute.
/// For example: opening raw socket or opening socket on low port number requires
/// root privileges. This makes it impossible or very hard to unit test the IfaceMgr.
/// In order to overcome this problem, it is recommended to create mock object derived
/// from this class that mimics the behavior of the real packet handling class making
/// IfaceMgr testable.
class PktFilter {
public:

    /// @brief Virtual Destructor
    virtual ~PktFilter() { }

    /// @brief Check if packet can be sent to the host without address directly.
    ///
    /// Checks if the Packet Filter class has capability to send a packet
    /// directly to the client having no address assigned. This capability
    /// is used by DHCPv4 servers which respond to the clients they assign
    /// addresses to. Not all classes derived from PktFilter support this
    /// because it requires injection of the destination host HW address to
    /// the link layer header of the packet.
    ///
    /// @return true of the direct response is supported.
    virtual bool isDirectResponseSupported() const = 0;

    /// @brief Open socket.
    ///
    /// @param iface interface descriptor
    /// @param addr address on the interface to be used to send packets.
    /// @param port port number.
    /// @param receive_bcast configure socket to receive broadcast messages
    /// @param send_bcast configure socket to send broadcast messages.
    ///
    /// @return created socket's descriptor
    virtual int openSocket(const Iface& iface,
                           const isc::asiolink::IOAddress& addr,
                           const uint16_t port,
                           const bool receive_bcast,
                           const bool send_bcast) = 0;

    /// @brief Receive packet over specified socket.
    ///
    /// @param iface interface
    /// @param socket_info structure holding socket information
    ///
    /// @return Received packet
    virtual Pkt4Ptr receive(const Iface& iface,
                            const SocketInfo& socket_info) = 0;

    /// @brief Send packet over specified socket.
    ///
    /// @param iface interface to be used to send packet
    /// @param sockfd socket descriptor
    /// @param pkt packet to be sent
    ///
    /// @return result of sending the packet. It is 0 if successful.
    virtual int send(const Iface& iface, uint16_t sockfd,
                     const Pkt4Ptr& pkt) = 0;
};

/// Pointer to a PktFilter object.
typedef boost::shared_ptr<PktFilter> PktFilterPtr;

} // namespace isc::dhcp
} // namespace isc

#endif // PKT_FILTER_H