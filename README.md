# Registration key toolset

Cross platform generator of a readable key from a customer ID and an IP address. Generates a 20 byte key, with a CRC that looks like this: C6D3-E2VO-DC22-2XTN
Needs 5 character customer ID. This is an example rather than a not a production level code. It does NOT use PKI for key generation.

* regnum/mk - generate a key
* regnum/rk - validate a key
* regkeybox - windows app for key validation
