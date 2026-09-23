# transientlab app using Qt6Widgets
This app is used to configure and calibrate **transientlab** devices.

## Serial port configuration

| Parameter		|	Value			|
|		-		|		-			|
| BaudRate 		| 	115200			|
| DataBits 		| 	8				|
| Parity 		| 	NoParity		|
| StopBits 		| 	OneStop			|
| FlowControl 	| 	NoFlowControl	|

All main microcontrollers should have ports configured like that.

### Message frame

```c
const char START = '@';
const char PARAMETER + ':';
const char VALUE = '=';
const char END   = '!';
```