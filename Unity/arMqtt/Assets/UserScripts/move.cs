//-----------------------------------------------------------------------------
//  File:       move.cs
//  
//	Author:		Edward U. Benitez Rendon
//  
//  Date:		11.07.2017
//  
//	Desc:		This program contains functions to receive the joystick status
//              from slave device by mqtt procotocol. Also, it allows to move 
//              the player according to the joystick status.
//-----------------------------------------------------------------------------
using UnityEngine;
// Import mqtt protocol
using uPLibrary.Networking.M2Mqtt;
using System;

public class move : MonoBehaviour {
    // Define the speed to move the player
    public float velocity = 50.0f;
    private MqttClient4Unity client;
    // Broker IP address
    public string brokerHostname = "255.255.255.255";
    // Broker port
    public int brokerPort = 1883;
    // User name and password of the broker
    public string usrNm = null;
    public string psw = null;
    // Topic to subscribe
    public string topic = "zone/sensor";
    // Store the joystick status from slave device
    private  string[] payload = {"xC", "yC"};

    // Use this for initialization
    void Start () {
        // Connect to the MQTT broker
        if (brokerHostname != null && usrNm != null && psw != null)
        {
            Connect();
            // Register topic in the client
            client.Subscribe(topic);
        }
    }
	
	// Update is called once per frame
	void Update () {
        // Listen status from slave device
        listenMqtt();
        // Move player according to the joystick status
        if (payload[1] == "yL")
        {
            // Move player to left
            gameObject.transform.Translate(-velocity * Time.deltaTime, 0, 0);
        }
        if (payload[1] == "yR")
        {
            // Move player to right
            gameObject.transform.Translate(velocity * Time.deltaTime, 0, 0);
        }
        if (payload[0] == "xD")
        {
            // Move player to backward
            gameObject.transform.Translate(0, 0, -velocity * Time.deltaTime);
        }
        if (payload[0] == "xU")
        {
            // Move player to forward
            gameObject.transform.Translate(0, 0, velocity * Time.deltaTime);
        }
    }

    /// <summary>
    /// Connect MQTT client with the broker
    /// </summary>
    public void Connect()
    {
        client = new MqttClient4Unity(brokerHostname, brokerPort, false, null);
        string clientId = Guid.NewGuid().ToString();
        client.Connect(clientId, usrNm, psw);
    }

    /// <summary>
    /// Listen payload from broker
    /// </summary>
    private void listenMqtt()
    {
        while (client.Count() > 0)
        {
            string s = client.Receive();
            payload = s.Split(',');
        }
    }
}
