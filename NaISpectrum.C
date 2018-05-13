NaISpectrum()
{
  TFile* f = TFile::Open("NaI_spectrum.root");
  TGraph* g = f->Get("spectrum");
  g->Draw("ap");

  double h = 6.63e-34; // J s
  double c = 3.00e8; // m/s
  double e = 1.6e-19; // C

  double nu = 300e-9; // m
  
  double E = h*c / nu /e;
  //   4.1328 eV

  cout << E << endl;

  double numin = 300;
  double numax = 900;

  double Emin = h*c/numax/e;
  double Emax = h*c/numin/e;

  double c = 0;
  vector<double> vecx, vecy;
  for (int i=0; i<20; ++i) {
    double E = Emin + (Emax-Emin) / (19) * i;
    c += g->Eval(E);
    vecx.push_back(E);
    vecy.push_back(g->Eval(E));
  }

  c *= (Emin-Emin) / 19;

  for (int i=0; i<vecx.size(); ++i) {
    vecy[i] = vecy[i] / c;
    cout << vecx[i] << " " << vecy[i] << endl;
  }

  
  
}
