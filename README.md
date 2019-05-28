# NO-BS UDP

Bypasses all Node.js logic and just sends the UDP packet. Fire and forget.

## Usage

```
import udp from 'nobsudp';

// (data:Buffer, host:string, port: string)
udp.Send(Buffer.from('abc'), '127.0.0.1', '9000');

```

## Roadmap

This is a test project to validate some assumptions, there's perhaps ways to improve this,
like storing connection information in a TLRU cache for faster repeated transmitions to a same host.

Feel free to contribute or leave a comment or suggestion. 