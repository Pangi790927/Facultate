u(S) = uMax * S / (Ks + Kd + S)
	uMax: 0.739258
	Ks: 0.00105301
	Kd: 0.00105301

u(S) = uMax * (1 - exp(-S / Ks))
	uMax: 0.735
	Ks: 1e-10

u(S) = uMax * S / (Ks + S) - Kis * (S - Sc)
	Results are invalid: -8
	uMax: nan
	Ks: nan
	Kis: nan
	Sc: nan

