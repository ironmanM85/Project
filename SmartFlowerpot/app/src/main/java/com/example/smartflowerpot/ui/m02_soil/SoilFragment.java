package com.example.smartflowerpot.ui.m02_soil;

import android.Manifest;
import android.bluetooth.BluetoothDevice;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.ViewModelProvider;

import com.example.smartflowerpot.MainActivity;
import com.example.smartflowerpot.MainViewModel;
import com.example.smartflowerpot.R;
import com.example.smartflowerpot.databinding.FragmentSoilBinding;
import com.example.smartflowerpot.firmata.ByteReader;
import com.example.smartflowerpot.firmata.FirmataBluetooth;

import me.aflak.bluetooth.Bluetooth;

public class SoilFragment extends Fragment {

    private FragmentSoilBinding binding;
    SoilViewModel model;
    MainViewModel mainModel;


    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        binding = FragmentSoilBinding.inflate(inflater, container, false);
        model = new ViewModelProvider(this).get(SoilViewModel.class);
        mainModel = new ViewModelProvider(requireActivity()).get(MainViewModel.class);

        permissionGranted();

        mainModel = new ViewModelProvider(this).get(MainViewModel.class);
        mainModel.bluetooth = new Bluetooth(requireActivity());
        mainModel.bluetooth.setReader(ByteReader.class);
        mainModel.firmata = new FirmataBluetooth(mainModel);
        mainModel.bluetooth.setDeviceCallback(mainModel.firmata);

        return binding.getRoot();
    }

    @Override
    public void onStart() {
        super.onStart();
        mainModel.bluetooth.onStart();
    }

    @Override
    public void onStop() {
        super.onStop();
        mainModel.bluetooth.onStop();
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        mainModel.bluetooth.disconnect();
        binding = null;
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        binding.btnBluetooth.setOnClickListener(v -> {
            if(mainModel.bluetooth.isConnected()){
                        Toast.makeText(getActivity(), "Disconnect", Toast.LENGTH_SHORT).show();
                mainModel.firmata.disEnableSoil();
                mainModel.bluetooth.disconnect();
                binding.btnBluetooth.setText("Connect");
            }
            else{
                Toast.makeText(getActivity(), "Connect", Toast.LENGTH_SHORT).show();
                mainModel.bluetooth.connectToName("Project");
                binding.btnBluetooth.setText("Disconnect");
                new Handler().postDelayed(new Runnable()
                {
                    @Override
                    public void run()
                    {
                        mainModel.firmata.enableSoil();
                    }
                }, 3000);// 3초 정도 딜레이를 준 후 시작
            }
        });

        mainModel.soilValue.observe(getViewLifecycleOwner(), value ->{
            binding.txtSoil.setText("Soil Moisture(Analog) = " + value);
        });

        mainModel.soilValueTrans.observe(getViewLifecycleOwner(), soilValueTrans ->{
            binding.txtSoiltrans.setText("Soil Moisture(%) = " + soilValueTrans + "%");
            if(soilValueTrans >= 25)
                binding.imgWater.setImageResource(R.drawable.ic_water_full);
            else
                binding.imgWater.setImageResource(R.drawable.ic_water_enough);
        });
    }

    void permissionGranted(){
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            requestPermissions(
                    new String[]{
                            Manifest.permission.BLUETOOTH,
                            Manifest.permission.BLUETOOTH_SCAN,
                            Manifest.permission.BLUETOOTH_ADVERTISE,
                            Manifest.permission.BLUETOOTH_CONNECT


                    },
                    1);
        } else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            requestPermissions(
                    new String[]{
                            Manifest.permission.BLUETOOTH

                    },
                    1);
        }
    }
}