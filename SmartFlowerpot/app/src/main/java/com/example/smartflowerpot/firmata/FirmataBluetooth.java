package com.example.smartflowerpot.firmata;

import android.bluetooth.BluetoothDevice;
import android.os.Build;
import android.util.Log;

import androidx.annotation.RequiresApi;

import com.example.smartflowerpot.MainViewModel;

import java.util.Arrays;

import me.aflak.bluetooth.Bluetooth;
import me.aflak.bluetooth.interfaces.DeviceCallback;

public class FirmataBluetooth extends Firmata implements DeviceCallback {
    MainViewModel mainModel;

    public FirmataBluetooth(MainViewModel model) {
        mainModel = model;

        reportVersionCallback = (major, minor) -> {
            Log.d("###", String.format("Firmata V%d.%d",major,minor));
        };

        analogMessageCallback = (pin, value) -> {
            Log.d("###", String.format("pin = %d, value = %d", pin, value));
            int soilValueTrans = map(value, 1023, 0, 0, 100);

            switch(pin){
                case 1 :
                    mainModel.soilValue.postValue(value);
                    mainModel.soilValueTrans.postValue(soilValueTrans);
                    mainModel.firmata.soilValue(soilValueTrans);
                    break;
                default:
                    break;
            }
        };
    }

    @Override
    public int write(byte[] buffer, int length) {
        mainModel.bluetooth.send(Arrays.copyOf(buffer, length));
        return length;
    }

    public void enableSoil(){
        reportAnalog(1,1);
    }

    public void disEnableSoil(){
        reportAnalog(1,0);
    }

    @Override
    public void onDeviceConnected(BluetoothDevice device) {
        Log.d("###", "onDeviceConnected..");
        mainModel.firmata.sendString("Connect");
    }

    @Override
    public void onDeviceDisconnected(BluetoothDevice device, String message) {
        Log.d("###", "onDeviceDisconnected..");
    }

    @Override
    public void onMessage(byte[] message) {
//        Log.d("###", "onMessage..");
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
            processInput(message,message.length);
        }
    }

    @Override
    public void onError(int errorCode) {
        Log.d("###", "onError..");
    }

    @Override
    public void onConnectError(BluetoothDevice device, String message) {
        Log.d("###", "onConnectError..");
    }
}
