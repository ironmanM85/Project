package com.example.smartflowerpot.ui.m03_temp_humid;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.example.smartflowerpot.databinding.FragmentTempHumidBinding;

public class Temp_Humid_Fragment extends Fragment {

    private FragmentTempHumidBinding binding;
    Temp_Humid_ViewModel model;

    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        model = new ViewModelProvider(this).get(Temp_Humid_ViewModel.class);
        binding = FragmentTempHumidBinding.inflate(inflater, container, false);

        return binding.getRoot();
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }
}