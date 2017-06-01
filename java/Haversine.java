class Haversine {
  private native double haversine(double lat1, double lon1, double lat2, double lon2);

  public static void main(String[] args) {
    System.out.println(new Haversine().haversine(-70.0, 42.01000, -70.0, 42.00000));
  }

  static {
    System.loadLibrary("Haversine");
  }
}
