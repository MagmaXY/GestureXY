void oledTick() {
if (millis() - gData.timer >= db[kk::ons]) db[kk::state] = false;
}
