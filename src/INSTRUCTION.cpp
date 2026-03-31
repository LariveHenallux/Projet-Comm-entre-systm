int * listInstruction(int temps, int degrefinal, int degreInit) {
    int * result = new int[temps * 5];
    int hertz = temps * 5;
    int step = (degrefinal - degreInit) / hertz; // Calculate the step size for each increment
    for (int i = 0; i < hertz; i++) {
        result[i] = degreInit + (step * i); // Initialize all elements to the corresponding step value
    }
    return result;
}
