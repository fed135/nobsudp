# NO-BS UDP

Bypasses all Node.js logic and just sends the UDP packet. Fire and forget.

## Usage

```
import udp from 'nobsudp';

// (data:Buffer, host:string, port: string)
udp.Send(Buffer.from('abc'), '127.0.0.1', '9000');

```