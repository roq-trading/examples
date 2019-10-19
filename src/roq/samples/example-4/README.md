# Example 4

## Objective

* Demonstrate how to place and maintain limit orders

## What does it do?

* Extends `example-3`
* Place limit order (rather than IOC)
* Maintain working orders and monitor for timeouts

## Requirements

The example requires access to one or more market gateways running
on the same host.

> *The simulator can also be used as a gateway*.

## How to use

The example requires the `name` option specifying the login name used
when accessing the gateway(s).

Further arguments are UNIX domain socket end-points for the gateways.

Example:

```bash
./roq-samples-example-4 --name test ~/deribit.sock ~/coinbase-pro.sock
```

> *The paths used for the UNIX domain sockets obviously depend on how
> you configure and launch your gateways*.