package com.example.smartflowerpot.firmata;

import android.os.Build;

import androidx.annotation.RequiresApi;

import java.util.LinkedList;
import java.util.List;
import java.util.function.BiConsumer;

public abstract class Firmata {         // 상속해서 사용
    /*
     * Firmata 명령어
     */
    static final byte DIGITAL_MESSAGE = (byte) 0x90; // send data for a digital port (collection of 8 pins)
    static final byte ANALOG_MESSAGE = (byte) 0xE0; // send data for an analog pin (or PWM)
    static final byte REPORT_ANALOG = (byte) 0xC0; // enable analog input by pin #
    static final byte REPORT_DIGITAL = (byte) 0xD0; // enable digital input by port pair
    //
    static final byte SET_PIN_MODE = (byte) 0xF4; // set a pin to INPUT/OUTPUT/PWM/etc
    static final byte SET_DIGITAL_PIN_VALUE = (byte) 0xF5; // set value of an individual digital pin
    //
    static final byte REPORT_VERSION = (byte) 0xF9; // report protocol version
    static final byte SYSTEM_RESET = (byte) 0xFF; // reset from MIDI
    //
    static final byte START_SYSEX = (byte) 0xF0; // start a MIDI Sysex message
    static final byte END_SYSEX = (byte) 0xF7; // end a MIDI Sysex message

    /*
     * Firmata 알고리즘
     */
    List<Byte> queue;
    boolean isConnect = false;

    public boolean isConnect() {
        return isConnect;
    }

    public BiConsumer<Integer, Integer> reportVersionCallback;
    public BiConsumer<Integer, Integer> analogMessageCallback;

    public Firmata() {
        this.queue = new LinkedList<Byte>();
    }

    public abstract int write(byte[] buffer, int length);

    public void setPinMode(int pin, int mode) {
        byte[] msg = new byte[3];
        msg[0] = SET_PIN_MODE;
        msg[1] = (byte) pin;
        msg[2] = (byte) mode;
        write(msg, msg.length);
    }

    public void setDigitalPinValue(int pin, int value) {
        byte[] msg = new byte[3];
        msg[0] = SET_DIGITAL_PIN_VALUE;
        msg[1] = (byte) pin;
        msg[2] = (byte) value;
        write(msg, msg.length);
    }

    public void reportAnalog(int pin, int enable) {
        byte[] msg = new byte[2];
        msg[0] = (byte) (REPORT_ANALOG | pin);
        msg[1] = (byte) enable;
        write(msg, msg.length);
    }

    public void reportDigital(int port, int enable) {
        byte[] msg = new byte[2];
        msg[0] = (byte) (REPORT_DIGITAL | port);
        msg[1] = (byte) enable;
        write(msg, msg.length);
    }

    public void requestVersion(){
        byte[] msg = new byte[] {(byte) 0xF9};
        write(msg,msg.length);
    }

    public void debug() {
        System.out.print("Queue = [");
        for (int i = 0; i < queue.size(); i++) {
            System.out.printf("%X ", queue.get(i));
        }
        System.out.println("]");
    }

    void soilValue(int value){
        byte[] packet = new byte[2+4+1];
        packet[0] = START_SYSEX;
        packet[1] = (byte) 0x01;

        byte first = (byte)((value & 0x0000FF00) >> 8);
        byte second = (byte)(value & 0x000000FF);

        packet[2] = (byte)(first & 0x7F);
        packet[3] = (byte)((first & 0x80) >> 7);

        packet[4] = (byte)(second & 0x7F);
        packet[5] = (byte)((second & 0x80) >> 7);

        packet[6] = END_SYSEX;
        write(packet, packet.length);
    }

    @RequiresApi(api = Build.VERSION_CODES.N)
    public void processInput(byte[] buffer, int length) {
        for (int i = 0; i < length; i++)
            queue.add(buffer[i]);

//		debug();

        while (queue.size() >= 3) {
            byte command = queue.get(0);
            switch (command) {
                case REPORT_VERSION:
                    if (reportVersionCallback != null) {
                        int major = queue.get(1);
                        int minor = queue.get(2);
                        reportVersionCallback.accept(major, minor);
                    }
                    queue.subList(0, 3).clear();
                    isConnect = true;
                    break;
                default:
                    break;
            }

            byte nibbleCommand = (byte) (command & 0xF0);
            switch (nibbleCommand) {
                case ANALOG_MESSAGE:
                    if (analogMessageCallback != null) {
                        int pin = command & 0x0F;
                        byte first = queue.get(1);
                        byte second = queue.get(2);
                        int value = second << 7 | first;
                        analogMessageCallback.accept(pin, value);
                    }
                    queue.subList(0, 3).clear();
                    break;
                default:
                    break;
            }
        }

    }

    public void sendString(String str) {
        byte[] data = str.getBytes();
        byte[] packet = new byte[2 + data.length * 2 + 1];
        packet[0] = START_SYSEX;
        packet[1] = (byte) 0x71;

        for (int i = 0; i < data.length; i++) {
            packet[2 + i * 2] = (byte) (data[i] & 0x7F);
            packet[2 + i * 2 + 1] = (byte) ((data[i] >> 7) & 0x01);
        }

        packet[packet.length - 1] = END_SYSEX;
        write(packet, packet.length);
    }

    public int map(int x, int in_min, int in_max, int out_min, int out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
}

