// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	DefaultHealth = 100;

	SetIsReplicated(true);
}


// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		// Only hook if we are server
		if (MyOwner->Role == ROLE_Authority)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
		}
	}

	Health = DefaultHealth;
	
}

void USHealthComponent::OnRep_Health(float OldHealth)
{
	float Damage = Health - OldHealth;

	OnHealthChanged.Broadcast(this, Health, Damage, nullptr, nullptr, nullptr);
}

void USHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
	{
		return;
	}

	//Update health clamped
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health changed : %s"), *FString::SanitizeFloat(Health));

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}

void USHealthComponent::Heal(float amount)
{
	if (amount <= 0 || Health <= 0)
	{
		return;
	}

	Health = FMath::Clamp(Health + amount, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health changed : %s {+%s}"), *FString::SanitizeFloat(Health), *FString::SanitizeFloat(amount));

	OnHealthChanged.Broadcast(this, Health, -amount, nullptr, nullptr, nullptr);
}

void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USHealthComponent, Health);
}
