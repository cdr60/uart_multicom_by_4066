# uart_multicom_by_4066
Communicate via one serial port between more than 2 arduinos
This is a test project to demonstrate if a solution is usable

Imagine, that you get 127 arduinos, they have to communicate to another one and this one have to communicate with all but not at the same time !
And imagine that nrf24, Loran, Wifi can not be used.
You cannot add 128 serial ports !

The IC4066 is a very cheap IC (about 0,50€) . It is a set of logic switches.
![cd4066be](https://github.com/cdr60/uart_multicom_by_4066/assets/104300119/a346800b-d2b3-4b0e-9594-f2155a751b65)


I've try to make 2 arduinos nano communicate qui an arduino mega and make the arduino mega capable to communicate with the nano that he needs.
And it works !

I've complicate the projet, adding some bytes to know wich arduino has sent datas and wich arduino was the recipients.
So each arduinos has an address.
Mega Arduo has de 0x08
Nano chan has 0x09 or 0x10 depending if D4 is grounded or not
And the stream is like this : 
First byte : Destination address
Second byte : Source address
Third, Forth , Fifth ans sixth : are random bytes (As Rfid use 4 bytes, it can be that type of data)

Earch Arduno nano got a push button. If you push it, the nano will send a byte to the Mega
The Arduino Mega got 2 push button : pushing one will make it send datas to one of the nano and pushing the other one will send dats to the other one.

Each Arduno nano got a Led : When receiving datas, it will the led blinking
The Arduino Mega got 2 Led : one Led by Arduino nano. This make us see the receiving data and wich nano has sent them.

The CD4066BE got 4 logical switchs.
Using the 4 control pins, we can make the connexion between the RX and the TX that we want.
And you've probably understood : using the psush button will activate the control pin that we need.

# Cable schematics
![Arduino_serial_4066_bb](https://github.com/cdr60/uart_multicom_by_4066/assets/104300119/6f72e744-7e2d-4a87-9575-0818c32ddfc8)


![Arduino_serial_4066_uno_bb](https://github.com/cdr60/uart_multicom_by_4066/assets/104300119/5ee22527-4ec5-48f8-8a17-e1be7f7a0cfa)






