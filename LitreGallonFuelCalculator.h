#pragma once

using namespace std;

class VehicleFuelConsumptionPerTrip {
private:
    double MPG = 0;
    double LitreOfFuelConsumed = 0;
    double GallonOfFuelConsumed = 0;
    double LastTimePumpedFuelMileage_Mile = 0;
    double ThisTimePumpedFuelMileage_Mile = 0;
    double DeltaPumpedFuelMileage_Mile = 0;
    double LastTimePumpedFuelMileage_Kilometer = 0;
    double ThisTimePumpedFuelMileage_Kilometer = 0;
    double DeltaPumpedFuelMileage_Kilometer = 0;
    double FuelPricePerLitre = 0;
    double FuelPricePerGallon = 0;
    string FuelType = "";
    double FuelCostPerMile = 0;
    double FuelCostPerKilometer = 0;
    double Estimate_Trip_in_Miles = 0;
    double Estimate_Trip_in_Kilometer = 0;
    double FuelCost_Estimate_Trip = 0;

public:
    void calculateVariables() {

        if (FuelPricePerGallon != 0 && FuelPricePerLitre == 0) {
            FuelPricePerLitre = FuelPricePerGallon * 0.22;
        }
        else if (FuelPricePerLitre != 0 && FuelPricePerGallon == 0) {
            FuelPricePerGallon = FuelPricePerLitre / 0.22;
        }
        if (Estimate_Trip_in_Miles != 0 && Estimate_Trip_in_Kilometer == 0) {
            Estimate_Trip_in_Kilometer = Estimate_Trip_in_Miles * 0.6213712;
        }
        else if (Estimate_Trip_in_Kilometer != 0 && Estimate_Trip_in_Miles == 0) {
            Estimate_Trip_in_Miles = Estimate_Trip_in_Kilometer / 0.6213712;
        }

        // Estimate DeltaPumpedFuelMileage in miles and kilometers to calculate MPG
        if (LastTimePumpedFuelMileage_Mile != 0 && LastTimePumpedFuelMileage_Kilometer == 0) {
            LastTimePumpedFuelMileage_Kilometer = LastTimePumpedFuelMileage_Mile / 0.6213712;
        }
        else if (LastTimePumpedFuelMileage_Kilometer != 0 && LastTimePumpedFuelMileage_Mile == 0) {
            LastTimePumpedFuelMileage_Mile = LastTimePumpedFuelMileage_Kilometer * 0.6213712;
        }
        if (ThisTimePumpedFuelMileage_Mile != 0 && ThisTimePumpedFuelMileage_Kilometer == 0) {
            ThisTimePumpedFuelMileage_Kilometer = ThisTimePumpedFuelMileage_Mile / 0.6213712;
        }
        else if (ThisTimePumpedFuelMileage_Kilometer != 0 && ThisTimePumpedFuelMileage_Mile == 0) {
            ThisTimePumpedFuelMileage_Mile = ThisTimePumpedFuelMileage_Kilometer * 0.6213712;
        }
        DeltaPumpedFuelMileage_Mile = ThisTimePumpedFuelMileage_Mile - LastTimePumpedFuelMileage_Mile;
        DeltaPumpedFuelMileage_Kilometer = ThisTimePumpedFuelMileage_Kilometer - LastTimePumpedFuelMileage_Kilometer;

        // Conversion from gallons to liters and vice versa
        if (GallonOfFuelConsumed != 0 && LitreOfFuelConsumed == 0) {
            LitreOfFuelConsumed = GallonOfFuelConsumed * 3.78541;
        }
        else if (LitreOfFuelConsumed != 0 && GallonOfFuelConsumed == 0) {
            GallonOfFuelConsumed = LitreOfFuelConsumed / 3.78541;
        }

        MPG = DeltaPumpedFuelMileage_Mile / (LitreOfFuelConsumed * 0.2199693);

        // Fuel cost per mile and per kilometer
        if (MPG != 0 && FuelPricePerGallon != 0) {
            FuelCostPerMile = FuelPricePerGallon / MPG;
            FuelCostPerKilometer = FuelCostPerMile / 1.60934;
        }
        else if (MPG != 0 && FuelPricePerLitre != 0) {
            FuelCostPerKilometer = FuelPricePerLitre / (MPG * 0.425144);
            FuelCostPerMile = FuelCostPerKilometer * 1.60934;
        }

        // Estimated fuel cost for the trip
        if (Estimate_Trip_in_Miles == 0 && Estimate_Trip_in_Kilometer != 0) {
            Estimate_Trip_in_Miles = Estimate_Trip_in_Kilometer * 1.60934;
        }
        FuelCost_Estimate_Trip = Estimate_Trip_in_Miles * FuelCostPerMile;
    }


    // Getter and setter methods for the variables

    double getMPG() const { //MPG
        return MPG;
    }

    void setMPG(double value) {
        MPG = value;
    }

    double getLitreOfFuelConsumed() const { //Litre of Fuel Consumed
        return LitreOfFuelConsumed;
    }

    void setLitreOfFuelConsumed(double value) {
        LitreOfFuelConsumed = value;
    }

    double getGallonOfFuelConsumed() const { //Gallon of Fuel Consumed
        return GallonOfFuelConsumed;
    }

    void setGallonOfFuelConsumed(double value) {
        GallonOfFuelConsumed = value;
    }

    double getLastTimePumpedFuelMileage_Mile() const { //Trip Mile
        return LastTimePumpedFuelMileage_Mile;
    }

    void setLastTimePumpedFuelMileage_Mile(double value) {
        LastTimePumpedFuelMileage_Mile = value;
    }

    double getThisTimePumpedFuelMileage_Mile() const {
        return ThisTimePumpedFuelMileage_Mile;
    }

    void setThisTimePumpedFuelMileage_Mile(double value) {
        ThisTimePumpedFuelMileage_Mile = value;
    }

    double getDeltaPumpedFuelMileage_Mile() const {
        return DeltaPumpedFuelMileage_Mile;
    }

    void setDeltaPumpedFuelMileage_Mile(double value) {
        DeltaPumpedFuelMileage_Mile = value;
    }

    double getLastTimePumpedFuelMileage_Kilometer() const { //Trip Kilometer
        return LastTimePumpedFuelMileage_Kilometer;
    }

    void setLastTimePumpedFuelMileage_Kilometer(double value) {
        LastTimePumpedFuelMileage_Kilometer = value;
    }

    double getThisTimePumpedFuelMileage_Kilometer() const {
        return ThisTimePumpedFuelMileage_Kilometer;
    }

    void setThisTimePumpedFuelMileage_Kilometer(double value) {
        ThisTimePumpedFuelMileage_Kilometer = value;
    }

    double getDeltaPumpedFuelMileage_Kilometer() const {
        return DeltaPumpedFuelMileage_Kilometer;
    }

    void setDeltaPumpedFuelMileage_Kilometer(double value) {
        DeltaPumpedFuelMileage_Kilometer = value;
    }

    double getFuelPricePerLitre() const { //Fuel Price Per Litre
        return FuelPricePerLitre;
    }

    void setFuelPricePerLitre(double value) {
        FuelPricePerLitre = value;
    }

    double getFuelPricePerGallon() const { //Fuel Price Per Gallon
        return FuelPricePerGallon;
    }

    void setFuelPricePerGallon(double value) {
        FuelPricePerGallon = value;
    }

    const string& getFuelType() const { //Fuel Type
        return FuelType;
    }

    void setFuelType(const string& value) {
        FuelType = value;
    }

    double getFuelCostPerMile() const { //Fuel Cost Per Mile
        return FuelCostPerMile;
    }

    void setFuelCostPerMile(double value) {
        FuelCostPerMile = value;
    }

    double getFuelCostPerKilometer() const { //Fuel Cost Per Kilometer
        return FuelCostPerKilometer;
    }

    void setFuelCostPerKilometer(double value) {
        FuelCostPerKilometer = value;
    }

    double getEstimate_Trip_in_Miles() const { //Estimate Trip in Mile
        return Estimate_Trip_in_Miles;
    }

    void setEstimate_Trip_in_Miles(double value) {
        Estimate_Trip_in_Miles = value;
    }

    double getEstimate_Trip_in_Kilometer() const { //Estimate Trip in Kilometer
        return Estimate_Trip_in_Kilometer;
    }

    void setEstimate_Trip_in_Kilometer(double value) {
        Estimate_Trip_in_Kilometer = value;
    }

    double getFuelCost_Estimate_Trip() const { //Fuel Cost for Estimate Trip in Mile
        return FuelCost_Estimate_Trip;
    }

    void setFuelCost_Estimate_Trip(double value) {
        FuelCost_Estimate_Trip = value;
    }
};
