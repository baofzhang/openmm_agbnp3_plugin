
#ifdef USE_CUTOFF
if (!isExcluded && r2 < CUTOFF_SQUARED) {
#else
if (!isExcluded) {
#endif
  // dielf = 2 * (conversion to kj/mol) * [-(1/2)(1/epsilon_in - 1/epsilon_out)]
  //real dielf = 2.0f*138.935456f*(-0.5f*(1.f - 1.f/80.0f));
  real dielf = -137.198763f;
  real qq = chargeParam1*chargeParam2;
  real qqf = dielf*qq;
  real bb = bornrParam1*bornrParam2;
  real etij = EXP(-0.25f*r2/bb);
  real fgb = RSQRT(r2 + bb*etij);
  real egb = qqf*fgb;
  
  real fgb3 = fgb*fgb*fgb;
  real mw = -qqf*(1.f-0.25f*etij)*fgb3; //virial

  tempEnergy += egb;
  dEdR -= mw; //looks like dEdR wants negative of the virial?
}

