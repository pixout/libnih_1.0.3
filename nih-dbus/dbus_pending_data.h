/* libnih
 *
 * Copyright © 2009 Scott James Remnant <scott@netsplit.com>.
 * Copyright © 2009 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef NIH_DBUS_PENDING_DATA_H
#define NIH_DBUS_PENDING_DATA_H

/**
 * NihDBusPendingData structures may be attached as the data member to a
 * DBusPendingCall to pass information to the notify function about the
 * connection the message was sent on, and the reply and error handlers
 * to be used.
 *
 * This is primarily used by code generated by nih-dbus-tool, which
 * calls nih_dbus_pending_data_new() in the method call function and
 * expects the structure to be passed to the notify function it sets on
 * the pending call.
 *
 * You may wish to use it in your own equivalent code.
 **/

#include <nih/macros.h>

#include <dbus/dbus.h>

#include <nih-dbus/dbus_message.h>


/**
 * NihDBusReplyHandler:
 * @data: data pointer passed to nih_dbus_pending_data_new(),
 * @message: NihDBusMessage context for reply (including connection):
 *
 * A D-Bus Reply Handler is expected to handle the receipt of a validated
 * reply to a method call, generally they will be called with additional
 * arguments representing the demarshalled data from the D-Bus reply
 * message itself.
 *
 * @message is a context containing both the connection the reply was
 * received on, and the reply message itself (which may have additional
 * data or flags).
 **/
typedef void (*NihDBusReplyHandler) (void *data, NihDBusMessage *message, ...);

/**
 * NihDBusErrorHandler:
 * @data: data pointer passed to nih_dbus_pending_data_new(),
 * @message: NihDBusMessage context for reply (including connection):
 *
 * A D-Bus Error Handler is expected to handle the receipt of an error
 * reply to a method call, or the receipt of an invalid reply.  The
 * error to be handled is raised when the handler is called; for a
 * reply received from the server it will be NIH_DBUS_ERROR, for the
 * receipt of an invalid reply it will be NIH_DBUS_INVALID_ARGS.
 *
 * @message is a context containing both the connection the reply was
 * received on, and the reply message itself (which may have additional
 * data or flags).
 **/
typedef void (*NihDBusErrorHandler) (void *data, NihDBusMessage *message);


/**
 * NihDBusPendingData:
 * @connection: D-Bus connection message message is pending on,
 * @handler: function to handle a valid reply,
 * @error_handler: function to handle errors,
 * @data: data to pass to @handler and @error_handler.
 *
 * This structure is used as a context for the processing of a message; the
 * primary reason for its existance is to be used as an nih_alloc() context
 * for any reply data.
 *
 * Instances are allocated automatically and passed to marshaller functions,
 * and freed on their return.
 **/
typedef struct nih_dbus_pending_data {
	DBusConnection *    connection;
	NihDBusReplyHandler handler;
	NihDBusErrorHandler error_handler;
	void *              data;
} NihDBusPendingData;


NIH_BEGIN_EXTERN

NihDBusPendingData *nih_dbus_pending_data_new (const void *parent,
					       DBusConnection *connection,
					       NihDBusReplyHandler handler,
					       NihDBusErrorHandler error_handler,
					       void *data)
	__attribute__ ((warn_unused_result));

NIH_END_EXTERN

#endif /* NIH_DBUS_PENDING_DATA_H */
