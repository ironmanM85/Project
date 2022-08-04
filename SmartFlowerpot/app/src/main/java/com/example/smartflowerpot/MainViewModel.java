package com.example.smartflowerpot;

import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.example.smartflowerpot.firmata.FirmataBluetooth;

import me.aflak.bluetooth.Bluetooth;

public class MainViewModel extends ViewModel {
    public Bluetooth bluetooth;
    public FirmataBluetooth firmata;

    public MutableLiveData<Integer> soilValue = new MutableLiveData<>(0);
    public MutableLiveData<Integer> soilValueTrans = new MutableLiveData<>(0);
    public MutableLiveData<Integer> fireValue = new MutableLiveData<>(0);
    public MutableLiveData<Integer> temp = new MutableLiveData<>(0);
    public MutableLiveData<Integer> humid = new MutableLiveData<>(0);
}
