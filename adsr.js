module.exports = class ADSR {
	constructor() {
		this.state = "attack"
		this.vol = 0
		this.peak = 127
		this.attack_rate = 1
		this.decay_rate = 2
		this.sustain = 63
		this.release_rate = 10
	}

	next() {
		switch (this.state) {
			case "attack": {
				this.vol += this.attack_rate
				if (this.vol > this.peak) {
					this.vol = this.peak
					this.state = "decay"
				}
				break
			}
			case "decay": {
				this.vol -= this.decay_rate
				if (this.vol < this.sustain) {
					this.vol = this.sustain
					this.state = "sustain"
				}
				break
			}
			case "done": {
			}
			case "sustain": {
				break
			}
			case "release": {
				this.vol -= this.release_rate
				if (this.vol < 0) {
					this.vol = 0
					this.state = "done"
				}
				break
			}
		}
		return this.vol
	}
}
