import streamlit as st
import pandas as pd

st.markdown(f"""

# Simple Calibration App

This is an application you can use to calibrate for **climateBOX** revC sensor in a simplified way.

""")


rv_raw = st.number_input("Enter your value for RV:", min_value=1.0, max_value=2.0, step=1e-3, format="%.3f")

tmp_raw = st.number_input("Enter your value for TMP:", min_value=1.0, max_value=2.0, step=1e-3, format="%.3f")

st.markdown(
    f"""
    * RV: {rv_raw}
    * TMP: {tmp_raw}
"""
)