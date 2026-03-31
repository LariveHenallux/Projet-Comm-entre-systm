int * ouverture(int temps, int degre) {
    int * result = new int[temps * 100];
    int hertz = temps * 100;
    int step = degre / hertz; // Calculate the step size for each increment
    for (int i = 0; i < hertz; i++) {
        result[i] = step * i; // Initialize all elements to the corresponding step value
    }
    return result;
}
