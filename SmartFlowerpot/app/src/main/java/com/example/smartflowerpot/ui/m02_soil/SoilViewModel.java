package com.example.smartflowerpot.ui.m02_soil;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class SoilViewModel extends ViewModel {

    private final MutableLiveData<String> mText;

    public SoilViewModel() {
        mText = new MutableLiveData<>();
        mText.setValue("This is gallery fragment");
    }

    public LiveData<String> getText() {
        return mText;
    }
}