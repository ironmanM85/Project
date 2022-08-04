package com.example.smartflowerpot.ui.m03_temp_humid;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class Temp_Humid_ViewModel extends ViewModel {

    private final MutableLiveData<String> mText;

    public Temp_Humid_ViewModel() {
        mText = new MutableLiveData<>();
        mText.setValue("This is slideshow fragment");
    }

    public LiveData<String> getText() {
        return mText;
    }
}